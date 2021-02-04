#pragma once
#include "HAcceptor.h"
#include "HChatUser.h"

class HLoginAcceptor : public HAcceptor
{
public:
	virtual bool AddSession(SOCKET sock, SOCKADDR_IN addr) override;

public:
	HLoginAcceptor();
	virtual ~HLoginAcceptor();
};

