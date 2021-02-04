#pragma once
#include "HNetUser.h"

struct HPacket
{
	HNetUser* pUser;
	UPACKET packet;
};

class HNetwork
{
public:
	std::list<HNetUser> m_UserList;
	SOCKET m_Sock;
	std::vector<HPacket> m_RecvPacket;
	CRITICAL_SECTION	m_cs;
	HANDLE m_hMutex;
	static bool g_bConnect;

public:
	bool	AddUser(SOCKET sock, SOCKADDR_IN addr);
	int		SendMsg(SOCKET sock, char* msg, int iLen, uint16_t type);
	void	AddPacket(HNetUser& user, UPACKET* packet);
	virtual bool	RecvData(HNetUser& user);
	bool	SendData(HNetUser& user, UPACKET& msg);
	virtual bool	DelUser(HNetUser& user);
	bool	MakePacket(UPACKET& packet, char* msg, int len, uint16_t type);

public:
	virtual bool	Frame() { return true; };
	virtual bool	Run();
	virtual bool	RecvUserList();
	virtual bool	BroadCasting();
	virtual bool	InitNetwork(std::string ip, int port);
	virtual bool	InitSocket(std::string ip, int port);
	virtual bool	DeleteNetwork();
	virtual bool	Accept();
	virtual void	PacketProcess();
	virtual bool	Process();

public:

	HNetwork();
	virtual ~HNetwork();
};

