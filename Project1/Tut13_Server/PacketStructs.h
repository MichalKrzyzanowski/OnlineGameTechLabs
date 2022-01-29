#pragma once
#include "PacketType.h"
#include "Packet.h"
#include "FileTransferData.h" //for FileTransferData::buffersize
#include <string> //for std::string
#include <memory> //for std::shared_ptr

namespace PS //Packet Structures Namespace
{
	class ChatMessage
	{
	public:
		ChatMessage(const std::string & str);
		std::shared_ptr<Packet> toPacket(); //Converts ChatMessage to packet
	private:
		std::string m_message;
	};
	class FileDataBuffer //Used when sending a file
	{
	public:
		std::shared_ptr<Packet> toPacket(); //Converts FileDataBuffer to Packet so that it can be appended to packet manager
		char m_databuffer[FileTransferData::m_bufferSize]; //buffer of data to be sent
		int m_size; //size of data buffer
	};
}