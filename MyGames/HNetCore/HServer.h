#pragma once
#include "HSessionMgr.h"
#include "HPacketPool.h"

class HServer : public HNetSingleton<HServer>, public HThread
{
private:
	friend class HNetSingleton<HServer>;
public:
	HPacketPool	 m_RecvPacketPool;
	HPacketPool	 m_SendPacketPool;
	HPacketPool	 m_SendBroadcastPacketPool;
public:
	typedef	void (HServer::*CallFuction)(HPacket& t);
	typedef std::map<int, CallFuction>::iterator FunctionIterator;
	std::map<int, CallFuction>    m_fnExecutePacket;
public:
	virtual  bool MakePacket(UPACKET& packet, char* msg, int iLen, uint16_t type);
	virtual  bool SendData(HNetUser& user, UPACKET& msg);
	virtual  bool Broadcastting();
	virtual  bool Run() override;
	virtual  bool Init();
public:
	void ChatMsg(HPacket& t);
	void MoveAction(HPacket& t);
	void Login(HPacket& t);
public:
	HServer();
	virtual ~HServer();
};
#define I_Server HServer::GetInstance()
