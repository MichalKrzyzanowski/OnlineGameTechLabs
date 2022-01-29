#include "Server.h"
#include "PacketStructs.h"
#pragma comment(lib,"ws2_32.lib") //Required for WinSock

bool Server::recvall(std::shared_ptr<Connection> connection, char * data, int totalbytes)
{
	int bytesReceived = 0; //Holds the total bytes received
	while (bytesReceived < totalbytes) //While we still have more bytes to recv
	{
		int retnCheck = recv(connection->m_socket, data + bytesReceived, totalbytes - bytesReceived, 0); //Try to recv remaining bytes
		if (retnCheck == SOCKET_ERROR || retnCheck == 0) //If there is a socket error while trying to recv bytes or if connection lost
			return false; //Return false - failed to recvall
		bytesReceived += retnCheck; //Add to total bytes received
	}
	return true; //Success!
}

bool Server::sendall(std::shared_ptr<Connection> connection, const char * data, const int totalBytes)
{
	int bytesSent = 0; //Holds the total bytes sent
	while (bytesSent < totalBytes) //While we still have more bytes to send
	{
		int retnCheck = send(connection->m_socket, data + bytesSent, totalBytes - bytesSent, 0); //Try to send remaining bytes
		if (retnCheck == SOCKET_ERROR) //If there is a socket error while trying to send bytes
			return false; //Return false - failed to sendall
		bytesSent += retnCheck; //Add to total bytes sent
	}
	return true; //Success!
}

bool Server::Getint32_t(std::shared_ptr<Connection> connection, std::int32_t & int32_t)
{
	if (!recvall(connection, (char*)&int32_t, sizeof(std::int32_t))) //Try to receive long (4 byte int)... If int fails to be recv'd
		return false; //Return false: Int not successfully received
	int32_t = ntohl(int32_t); //Convert long from Network Byte Order to Host Byte Order
	return true;//Return true if we were successful in retrieving the int
}

bool Server::GetPacketType(std::shared_ptr<Connection> connection, PacketType & packetType)
{
	std::int32_t packettype_int;
	if (!Getint32_t(connection, packettype_int)) //Try to receive packet type... If packet type fails to be recv'd
		return false; //Return false: packet type not successfully received
	packetType = (PacketType)packettype_int;
	return true;//Return true if we were successful in retrieving the packet type
}

void Server::SendString(std::shared_ptr<Connection> connection, const std::string & str)
{
	PS::ChatMessage message(str);
	connection->m_pm.Append(message.toPacket());
}

bool Server::GetString(std::shared_ptr<Connection> connection, std::string & str)
{
	std::int32_t bufferlength; //Holds length of the message
	if (!Getint32_t(connection, bufferlength)) //Get length of buffer and store it in variable: bufferlength
		return false; //If get int fails, return false
	if (bufferlength == 0) return true;
	str.resize(bufferlength); //resize string to fit message
	return recvall(connection, &str[0], bufferlength);
}
