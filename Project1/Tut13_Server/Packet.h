#pragma once
#include "PacketType.h"
#include <string>
#include <cstdint>
#include <vector>
#include <memory>
class Packet
{
public:
	Packet();
	Packet(const char * buffer, const int size); //Will use existing allocated buffer and create packet from it
												 //Packet(const Packet & p); //Will allocate new buffer but copy buffer from packet argument
	Packet(const PacketType p); //Used for when sending a packet that only contains a packet type (Ex. End of File Packet)
	Packet(const std::shared_ptr<Packet> p);
	void Append(const std::shared_ptr<Packet> p);
	void Append(const PacketType p);
	void Append(const std::int32_t int32);
	void Append(const std::size_t p);
	void Append(const Packet & p);
	void Append(const std::string & str);
	void Append(const char * buffer, const int size); //Will use existing allocated buffer and create packet from it
	std::vector<int8_t> m_buffer; //Packet Buffer	
};