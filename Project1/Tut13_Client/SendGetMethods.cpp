#include "Client.h"
#pragma comment(lib,"ws2_32.lib") //Required for WinSock
#include "PacketStructs.h"
bool Client::recvall(char * data, int totalBytes)
{
	int bytesReceived = 0; //Holds the total bytes received
	while (bytesReceived < totalBytes) //While we still have more bytes to recv
	{
		int RetnCheck = recv(m_connection, data + bytesReceived, totalBytes - bytesReceived, 0); //Try to recv remaining bytes
		if (RetnCheck == SOCKET_ERROR || RetnCheck == 0) //If there is a socket error while trying to recv bytes or if connection is lost
			return false; //Return false - failed to recvall
		bytesReceived += RetnCheck; //Add to total bytes received
	}
	return true; //Success!
}

bool Client::sendall(const char * data, const int totalBytes)
{
	int bytesSent = 0; //Holds the total bytes sent
	while (bytesSent < totalBytes) //While we still have more bytes to send
	{
		int RetnCheck = send(m_connection, data + bytesSent, totalBytes - bytesSent, 0); //Try to send remaining bytes
		if (RetnCheck == SOCKET_ERROR) //If there is a socket error while trying to send bytes
			return false; //Return false - failed to sendall
		bytesSent += RetnCheck; //Add to total bytes sent
	}
	return true; //Success!
}

bool Client::Getint32_t(std::int32_t & int32_t)
{
	if (!recvall((char*)&int32_t, sizeof(std::int32_t))) //Try to receive int... If int fails to be recv'd
		return false; //Return false: Int not successfully received
	int32_t = ntohl(int32_t); //Convert long from Network Byte Order to Host Byte Order
	return true;//Return true if we were successful in retrieving the int
}

bool Client::GetPacketType(PacketType & packetType)
{
	std::int32_t packetType_int;
	if (!Getint32_t(packetType_int))//Try to receive PacketType type... If PacketType type fails to be recv'd
		return false; //Return false: PacketType type not successfully received
	packetType = (PacketType)packetType_int;
	return true;//Return true if we were successful in retrieving the PacketType type
}

void Client::SendString(const std::string & str)
{
	PS::ChatMessage cm(str);
	m_pm.Append(cm.toPacket());
}

bool Client::GetString(std::string & str)
{
	int32_t bufferlength; //Holds length of the message
	if (!Getint32_t(bufferlength)) //Get length of buffer and store it in variable: bufferlength
		return false; //If get int fails, return false
	if (bufferlength == 0) return true;
	str.resize(bufferlength); //resize string to fit message
	return recvall(&str[0], bufferlength);
}