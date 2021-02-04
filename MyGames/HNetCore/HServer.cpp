#include "HServer.h"

bool HServer::MakePacket(UPACKET& packet,
	char* msg, int iLen, uint16_t type)
{
	packet.ph.iotype = 0;
	packet.ph.len = PACKET_HEADER_SIZE + iLen;
	packet.ph.type = type;
	packet.ph.time = time(NULL);
	memcpy(&packet.msg, msg, iLen);
	return true;
}
bool HServer::SendData(HNetUser& user, UPACKET& msg)
{
	DWORD dwSendByte;
	int iRet;
	user.m_wsaSendBuf.buf = (char*)&msg;
	user.m_wsaSendBuf.len = msg.ph.len;
	ZeroMemory(&user.m_ovSend, sizeof(OVERLAPPED));
	user.m_ovSend.iType = OVERLAPPED2::SEND_MODE;

	iRet = WSASend(user.m_Sock, &user.m_wsaSendBuf, 1,
		&dwSendByte, 0, (LPOVERLAPPED)&user.m_ovSend, NULL);

	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}
bool HServer::Run()
{
	while (m_bStarted)
	{
#pragma region m_RecvPacketPool
		m_RecvPacketPool.Lock();
		std::vector<HPacket>::iterator senditer;
		for (senditer = m_RecvPacketPool.m_list.begin();
			senditer != m_RecvPacketPool.m_list.end();
			senditer++)
		{
			UPACKET* packet = (UPACKET*)&senditer->packet;
			FunctionIterator calliter = m_fnExecutePacket.find(packet->ph.type);
			if (calliter != m_fnExecutePacket.end())
			{
				CallFuction call = calliter->second;
				(this->*call)(*senditer);
			}
		}
		m_RecvPacketPool.Unlock();

		m_RecvPacketPool.Clear();
#pragma endregion m_RecvPacketPool
#pragma region m_SendPacketPool
		{
			m_SendPacketPool.Lock();
			std::vector<HPacket>::iterator senditer;
			for (senditer = m_SendPacketPool.m_list.begin();
				senditer != m_SendPacketPool.m_list.end();
				senditer++)
			{
				HNetUser* pUser = senditer->pUser;
				if (SendData(*senditer->pUser, senditer->packet) == false)
				{
					pUser->m_bExit = true;
					I_Sesstion.DelUser(pUser);
				}
			}
			m_SendPacketPool.Unlock();
			m_SendPacketPool.Clear();
			/*TLock lock((TServerObj*)&I_Session);
			std::map<SOCKET, TNetUser*>::iterator iter;
			for (iter = I_Session.m_UserList.begin();
				iter != I_Session.m_UserList.end();
				)
			{
				TNetUser* pUser = iter->second;
				bool bDelete = false;
				std::vector<UPACKET>::iterator senditer;
				for (senditer = pUser->m_SendPacket.begin();
					senditer != pUser->m_SendPacket.end();
					senditer++)
				{
					if (SendData(*pUser, *senditer) == false)
					{
						pUser->m_bExit = true;
						bDelete = true;
						break;
					}
				}
				pUser->m_SendPacket.clear();

				if (bDelete == true)
				{
					I_Session.CloseUser(pUser);
					iter = I_Session.m_UserList.erase(iter);
				}
				else
				{
					iter++;
				}
			}
			*/
		}
#pragma endregion m_SendPacketPool
#pragma region m_SendBroadcastPacketPool
		{
			HLock lock((HServerObj*)&I_Sesstion);
			Broadcastting();
		}
#pragma endregion m_SendBroadcastPacketPool
	}
	return false;
}
bool HServer::Init()
{
	m_fnExecutePacket[PACKET_CHAT_MSG] = &HServer::ChatMsg;
	m_fnExecutePacket[PACKET_USER_POSITION] = &HServer::MoveAction;
	m_fnExecutePacket[PACKET_LOGIN_REQ] = &HServer::Login;
	return false;
}
bool HServer::Broadcastting()
{
	m_SendBroadcastPacketPool.Lock();

	std::map<SOCKET, HNetUser*>::iterator iter;
	for (iter = I_Sesstion.m_UserList.begin();
		iter != I_Sesstion.m_UserList.end();
		)
	{
		HNetUser* pUser = iter->second;
		bool bDelete = false;
		std::vector<HPacket>::iterator senditer;
		for (senditer = m_SendBroadcastPacketPool.m_list.begin();
			senditer != m_SendBroadcastPacketPool.m_list.end();
			senditer++)
		{
			if (SendData(*pUser, senditer->packet) == false)
			{
				bDelete == true;
				break;
			}
		}
		if (bDelete == true)
		{
			I_Sesstion.CloseUser(pUser);
			iter = I_Sesstion.m_UserList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	m_SendBroadcastPacketPool.Unlock();
	m_SendBroadcastPacketPool.Clear();

	return true;
}

HServer::HServer()
{
	CreateThread();
}

HServer::~HServer()
{

}
