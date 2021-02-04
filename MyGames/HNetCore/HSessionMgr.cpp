#include "HSessionMgr.h"

bool HSessionMgr::AddUser(HNetUser* pUser)
{
	HLock lock(this);
	m_UserList[pUser->m_Sock] = pUser;

	return true;
}

bool HSessionMgr::CloseUser(HNetUser * pUser)
{
	if (pUser == nullptr) return true;

	shutdown(pUser->m_Sock, SD_SEND);
	closesocket(pUser->m_Sock);
	delete pUser;
	pUser = nullptr;

	return true;

	return false;
}

bool HSessionMgr::DelUser(HNetUser* user)
{
	HLock lock(this);

	printf("\nÇØÁ¦ = %s:%d", inet_ntoa(user->m_addr.sin_addr),
		ntohs(user->m_addr.sin_port));

	CloseUser(user);
	std::map<SOCKET, HNetUser*>::iterator iter = m_UserList.find(user->m_Sock);
	if (iter != m_UserList.end())
	{
		m_UserList.erase(iter);
	}

	return true;
}

bool HSessionMgr::Run()
{
	return true;
}

HSessionMgr::HSessionMgr()
{
	CreateThread();
}

HSessionMgr::~HSessionMgr()
{

}
