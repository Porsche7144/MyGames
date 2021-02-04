#include "HServer.h"

void HServer::ChatMsg(HPacket& t)
{
	HChatMsg* pMsg = (HChatMsg*)&t.packet.msg;
	printf("\n[%s]%s:%d", pMsg->szName,
		pMsg->buffer, pMsg->iCnt);
	m_SendBroadcastPacketPool.AddPacket(t);
}
void HServer::MoveAction(HPacket& t)
{
	HEnemyPos* pMsg = (HEnemyPos*)&t.packet.msg;
	printf("\n%d=[%10.4f:%10.4f:%10.4f", pMsg->index,
		pMsg->p[0],
		pMsg->p[1],
		pMsg->p[2]);
	//std::cout << PACKET_USER_POSITION << std::endl;
	m_SendBroadcastPacketPool.AddPacket(t);
}

void HServer::Login(HPacket& t)
{
	UPACKET sendPacket;
	T_STR szID = L"kgca";
	T_STR szPS = L"game";
	HLogin* login = (HLogin*)t.packet.msg;
	T_STR szIDUser = to_mw(login->szID);
	T_STR szPSUser = to_mw(login->szPW);
	HLoginResult ret;
	if (szID == szIDUser && szPS == szPSUser)
	{
		ret.iRet = 1;
	}
	else
	{
		ret.iRet = 0;
	}
	MakePacket(sendPacket, (char*)&ret, sizeof(HLoginResult),
		PACKET_LOGIN_ACK);
	if (t.pUser != nullptr)
	{
		//t.pUser->m_SendPacket.push_back(sendPacket);
		HPacket tPacket = t;
		tPacket.pUser = t.pUser;
		tPacket.packet = sendPacket;
		m_SendPacketPool.AddPacket(tPacket);
	}
	std::cout << PACKET_LOGIN_REQ << std::endl;
}