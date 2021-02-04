#pragma once
#include "HNetwork.h"
class HNetworkClient : public HNetwork
{
	HANDLE g_hSendThread;

public:
	virtual bool	InitNetwork(std::string ip, int port) override;
	virtual bool	InitSocket(std::string ip, int port) override;
	virtual void	PacketProcess() override;
	virtual bool	RecvData(HNetUser& user) override;

public:
	void SendLoginData(SOCKET sock, std::string id, std::string pw);

	HNetworkClient();
	virtual ~HNetworkClient();
};

