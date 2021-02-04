#include "HPacketPool.h"

void	HPacketPool::Lock()
{
	WaitForSingleObject(m_hMutexPacketPool, INFINITE);
}
void	HPacketPool::Unlock()
{
	ReleaseMutex(m_hMutexPacketPool);
}
bool HPacketPool::MakePacket(UPACKET& packet,
	char* msg, int iLen, uint16_t type)
{
	packet.ph.iotype = 0;
	packet.ph.len = PACKET_HEADER_SIZE + iLen;
	packet.ph.type = type;
	packet.ph.time = time(NULL);
	memcpy(&packet.msg, msg, iLen);
	return true;
}
void	HPacketPool::AddPacket(HPacket& packet)
{
	Lock();
	m_list.push_back(packet);
	Unlock();
}
void	HPacketPool::Clear()
{
	Lock();
	m_list.clear();
	Unlock();
}
HPacketPool::HPacketPool()
{
	m_hMutexPacketPool = CreateMutex(NULL, FALSE, L"packetpool");
}

HPacketPool::~HPacketPool()
{
	CloseHandle(m_hMutexPacketPool);
}