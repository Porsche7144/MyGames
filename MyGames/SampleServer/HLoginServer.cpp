#include "HLoginServer.h"
#include "HIocp.h"
#include "HServer.h"

bool HLoginServer::Init()
{
	I_Server.Init();
	I_IOCP.Init();
	m_Acceptor.InitNetwork("192.168.25.53", 10000);
	m_Acceptor.CreateThread();
	m_bRun = true;
	return false;
}

bool HLoginServer::Run()
{
	while (m_bRun)
	{
		Sleep(10);
	}
	WaitForSingleObject((HANDLE)m_Acceptor.m_hThread, INFINITE);
	return false;
}

bool HLoginServer::Release()
{
	return false;
}

HLoginServer::HLoginServer() :m_bRun(false)
{
}

HLoginServer::~HLoginServer()
{
	I_IOCP.Release();
}

