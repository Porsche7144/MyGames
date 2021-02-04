#pragma once
#include "HNetUser.h"
class HPacketPool : public HServerObj
{
public:
	HANDLE						m_hMutexPacketPool;
	std::vector<HPacket>	    m_list;
public:
	void	AddPacket(HPacket& packet);
	void	Clear();
	void	Lock();
	void	Unlock();
	bool	MakePacket(UPACKET& packet, char* msg, int iLen, uint16_t type);
public:
	HPacketPool();
	virtual ~HPacketPool();
};

