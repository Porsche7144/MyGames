#pragma once
#include "HThread.h"
class HAcceptor : public HThread
{
public:
	SOCKET		m_Sock;
public:
	virtual bool Run() override;
	virtual bool Accept();
	virtual bool AddSession(SOCKET sock, SOCKADDR_IN addr);

	virtual bool InitNetwork(std::string ip, int iPort);
	virtual bool DeleteNetwork();

	virtual bool InitSocket(std::string ip, int iPort);
	virtual bool CloseSocket();
public:
	HAcceptor();
	virtual ~HAcceptor();
};