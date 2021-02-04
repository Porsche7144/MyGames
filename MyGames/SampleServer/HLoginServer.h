#pragma once
#include "HLoginAcceptor.h"

class HLoginServer
{
public:
	HLoginAcceptor	m_Acceptor;
	bool			m_bRun;
public:
	bool			Init();
	bool			Run();
	bool			Release();
public:
	HLoginServer();
	virtual ~HLoginServer();
};
