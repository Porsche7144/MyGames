#pragma once
#include "HNetwork.h"
class HNetworkClient : public HNetwork
{

public:
	HNetUser m_User;
	std::vector<UPACKET> m_sendPacket;
	bool m_bLogin;

public:
	virtual bool	InitNetwork(std::string ip, int port) override;
	virtual bool	InitSocket(std::string ip, int port) override;
	virtual void	PacketProcess() override;
	virtual bool	RecvData(HNetUser& user) override;
	virtual bool	SendData(HNetUser& user, UPACKET& msg);
	virtual bool	SendPackets(HNetUser& user);
	virtual bool	MsgEvent(MSG msg);


public:
	void SendLoginData(SOCKET sock, std::string id, std::string pw);

	HNetworkClient();
	~HNetworkClient();
};


class HNetworkClientSelect : public HNetworkClient
{

	FD_SET m_rSet;
	FD_SET m_wSet;
	FD_SET m_eSet;

public:
	virtual bool	InitNetwork(std::string ip, int port) override;
	virtual bool	InitSocket(std::string ip, int port) override;
	virtual bool	Frame() override;


public:

	HNetworkClientSelect() {};
	~HNetworkClientSelect() {};
};


class HNetworkClientEventSelect : public HNetworkClient
{

	HANDLE	m_EventArray[WSA_MAXIMUM_WAIT_EVENTS];
	int		m_iArrayCount;

public:
	virtual bool	InitNetwork(std::string ip, int port) override;
	virtual bool	InitSocket(std::string ip, int port) override;
	virtual bool	Frame() override;

public:

	HNetworkClientEventSelect() { m_iArrayCount = 0; };
	~HNetworkClientEventSelect() {};
};
