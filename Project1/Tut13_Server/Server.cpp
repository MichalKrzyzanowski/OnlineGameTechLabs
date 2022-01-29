#include "Server.h"
#include <iostream>
#include "PacketStructs.h"
#pragma comment(lib,"ws2_32.lib") //Required for WinSock

void Server::ClientHandlerThread(Server & server, std::shared_ptr<Connection> connection) //ID = the index in the SOCKET connections array
{
	PacketType packettype;
	while (true)
	{
		if (server.m_terminateThreads == true)
			break;
		if (!server.GetPacketType(connection, packettype)) //Get packet type
			break; //If there is an issue getting the packet type, exit this loop
		if (!server.ProcessPacket(connection, packettype)) //Process packet (packet type)
			break; //If there is an issue processing the packet, exit this loop
	}
	std::cout << "Lost connection to client ID: " << connection->m_ID << std::endl;
	server.DisconnectClient(connection); //Disconnect this client and clean up the connection if possible
	return;
}

void Server::PacketSenderThread(Server & server) //Thread for all outgoing packets
{
	while (true)
	{
		if (server.m_terminateThreads == true)
			break;
		std::shared_lock<std::shared_mutex> lock(server.m_mutex_connectionMgr);
		for (auto conn : server.m_connections) //for each connection...
		{
			if (conn->m_pm.HasPendingPackets()) //If there are pending packets for this connection's packet manager
			{
				std::shared_ptr<Packet> p = conn->m_pm.Retrieve(); //Retrieve packet from packet manager
				if (!server.sendall(conn, (const char*)(&p->m_buffer[0]), p->m_buffer.size())) //send packet to connection
				{
					std::cout << "Failed to send packet to ID: " << conn->m_ID << std::endl; //Print out if failed to send packet
				}
			}
		}
		Sleep(5);
	}
	std::cout << "Ending Packet Sender Thread..." << std::endl;
}

void Server::DisconnectClient(std::shared_ptr<Connection> connection) //Disconnects a client and cleans up socket if possible
{
	std::lock_guard<std::shared_mutex> lock(m_mutex_connectionMgr); //Lock connection manager mutex since we are possible removing element(s) from the vector
	connection->m_pm.Clear(); //Clear out all remaining packets in queue for this connection
	closesocket(connection->m_socket); //Close the socket for this connection
	m_connections.erase(std::remove(m_connections.begin(), m_connections.end(), connection)); //Remove connection from vector of connections
	std::cout << "Cleaned up client: " << connection->m_ID << "." << std::endl;
	std::cout << "Total connections: " << m_connections.size() << std::endl;
}