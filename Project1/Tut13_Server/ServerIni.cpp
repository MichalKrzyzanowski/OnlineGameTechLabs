#include "Server.h"
#include <WS2tcpip.h>
#include <iostream>
#pragma comment(lib,"ws2_32.lib") //Required for WinSock

Server::Server(int port, bool loopBacktoLocalHost) //Port = port to broadcast on. loopBacktoLocalHost = true if loopback to local host (nobody else can connect), false if loop back to any addr (anyone can connect) [assumes port is properly forwarded in router settings)
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) //If WSAStartup returns anything other than 0, then that means an error has occured in the WinSock Startup.
	{
		MessageBoxA(0, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	if (loopBacktoLocalHost) //If server is to loopback to local host (others cannot connect)
		inet_pton(AF_INET, "127.0.0.1", &m_addr.sin_addr.s_addr);
	else
		m_addr.sin_addr.s_addr = htonl(INADDR_ANY); //loopback to any addr so others can connect

	m_addr.sin_port = htons(port); //Port
	m_addr.sin_family = AF_INET; //IPv4 Socket

	m_sListen = socket(AF_INET, SOCK_STREAM, 0); //Create socket to listen for new connections
	if (bind(m_sListen, (SOCKADDR*)&m_addr, sizeof(m_addr)) == SOCKET_ERROR) //Bind the address to the socket, if we fail to bind the address..
	{
		std::string ErrorMsg = "Failed to bind the address to our listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
		MessageBoxA(0, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	if (listen(m_sListen, SOMAXCONN) == SOCKET_ERROR) //Places sListen socket in a state in which it is listening for an incoming connection. Note:SOMAXCONN = Socket Oustanding Max connections, if we fail to listen on listening socket...
	{
		std::string ErrorMsg = "Failed to listen on listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
		MessageBoxA(0, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	m_IDCounter = 0;
	std::thread PST(PacketSenderThread, std::ref(*this));
	PST.detach();
	m_threads.push_back(&PST);
}

bool Server::ListenForNewConnection()
{
	int addrlen = sizeof(m_addr);
	SOCKET newConnectionSocket = accept(m_sListen, (SOCKADDR*)&m_addr, &addrlen); //Accept a new connection
	if (newConnectionSocket == 0) //If accepting the client connection failed
	{
		std::cout << "Failed to accept the client's connection." << std::endl;
		return false;
	}
	else //If client connection properly accepted
	{
		std::lock_guard<std::shared_mutex> lock(m_mutex_connectionMgr); //Lock connection manager mutex since we are adding an element to connection vector
		std::shared_ptr<Connection> newConnection(std::make_shared<Connection>(newConnectionSocket));
		m_connections.push_back(newConnection); //push new connection into vector of connections
		newConnection->m_ID = m_IDCounter; //Set ID for this connection
		m_IDCounter += 1; //Increment ID Counter...
		std::cout << "Client Connected! ID:" << newConnection->m_ID << std::endl;
		std::thread CHT(ClientHandlerThread, std::ref(*this), newConnection);
		CHT.detach();
		m_threads.push_back(&CHT);
		return true;
	}
}

Server::~Server()
{
	m_terminateThreads = true;
	for (std::thread * t : m_threads) //Wait for all created threads to end...
	{
		t->join();
	}
}
