#pragma once
#include "Packet.h" //for Packet
#include <queue> //for std::queue
#include <mutex> //for std::mutex
#include <memory> //for std::shared_ptr

class PacketManager
{
private:
	std::queue<std::shared_ptr<Packet>> m_queue_packets;
	std::mutex m_mutex_packets;
public:
	void Clear();
	bool HasPendingPackets();
	void Append(std::shared_ptr<Packet> p);
	std::shared_ptr<Packet> Retrieve();
};