#include "HLoginAcceptor.h"
#include "HIocp.h"
#include "HSessionMgr.h"

bool HLoginAcceptor::AddSession(SOCKET sock, SOCKADDR_IN addr)
{
	HLock lock(this);
	HChatUser* pUser = new HChatUser;
	pUser->m_Sock = sock;
	pUser->m_addr = addr;
	I_Sesstion.AddUser(pUser);

	I_IOCP.SetBind(sock, (ULONG_PTR)pUser);

	pUser->WaitRecieve();

	printf("\n¡¢º” = %s:%d",
		inet_ntoa(addr.sin_addr),
		ntohs(addr.sin_port));

	return true;
}

HLoginAcceptor::HLoginAcceptor()
{
}

HLoginAcceptor::~HLoginAcceptor()
{
}
