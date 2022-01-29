#include "Packet.h"
#include <cstdint> //Required to use std::std::int32_t
#include <WinSock2.h> //for htonl
Packet::Packet()
{
}

Packet::Packet(const char * dataBuffer, const int size)
{
	Append(dataBuffer, size);
}

Packet::Packet(const std::shared_ptr<Packet> p)
{
	Append(p);
}

void Packet::Append(const std::shared_ptr<Packet> p)
{
	Append((const char*)&(p->m_buffer[0]), p->m_buffer.size());
}

Packet::Packet(PacketType pt)
{
	Append(pt);
}

void Packet::Append(const char * dataBuffer, const int size)
{
	m_buffer.insert(m_buffer.end(), dataBuffer, dataBuffer + size);
}

void Packet::Append(const std::string & str)
{
	Append(str.c_str(), str.size());
}

void Packet::Append(const Packet & p) //Allocate new block for buffer
{
	Append((const char*)&p.m_buffer, p.m_buffer.size());
}

void Packet::Append(std::int32_t int32)
{
	std::int32_t val = htonl((std::int32_t)int32);
	Append((const char*)&val, sizeof(std::int32_t));
}

void Packet::Append(PacketType pt)
{
	Append((std::int32_t)pt);
}

void Packet::Append(std::size_t s)
{
	Append((std::int32_t)s);
}
