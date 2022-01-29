#include "Client.h"
#include <Ws2tcpip.h> //for inet_pton
#pragma comment(lib,"ws2_32.lib") //Required for WinSock
#include <iostream> //for std::cout

Client::Client(const char * ip, const int port)
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(0, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}


	inet_pton(AF_INET, ip, &m_addr.sin_addr.s_addr); //Server Address [127.0.0.1] = localhost
	m_addr.sin_port = htons(port); //Port 
	m_addr.sin_family = AF_INET; //IPv4 Socket
}

bool Client::Connect()
{
	m_connection = socket(AF_INET, SOCK_STREAM, 0); //Set Connection socket
	int sizeofaddr = sizeof(m_addr);
	if (connect(m_connection, (SOCKADDR*)&m_addr, sizeofaddr) != 0) //If we are unable to connect...
	{
		MessageBoxA(0, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	std::cout << "Connected!" << std::endl;
	m_pst = std::thread(PacketSenderThread, std::ref(*this)); //Create thread to send packets
	m_pst.detach();
	m_ct = std::thread(ClientThread, std::ref(*this)); //Create thread to listen to server
	m_ct.detach();
	return true;
}

bool Client::CloseConnection()
{
	m_terminateThreads = true;
	if (closesocket(m_connection) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK) //If socket error is that operation is not performed on a socket (This happens when the socket has already been closed)
			return true; //return true since connection has already been closed

		std::string ErrorMessage = "Failed to close the socket. Winsock Error: " + std::to_string(WSAGetLastError()) + ".";
		MessageBoxA(0, ErrorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

Client::~Client()
{
	CloseConnection();
	m_pst.join();
	m_ct.join();
}
