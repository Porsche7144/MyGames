#include "HNetworkClient.h"
#include "Hstd.h"

#define NETWORK_MSG WM_USER+80

static int g_iChatCount = 0;
bool g_bConnect = false;

bool HNetworkClient::SendData(HNetUser& user, UPACKET& msg)
{
	while (user.iSendSize < msg.ph.len)
	{
		user.iSendSize += send(m_Sock, (char*)&msg, msg.ph.len - user.iSendSize, 0);

		if (user.iSendSize == 0)
		{
			return false;
		}
		if (user.iSendSize == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return false;
			}
			user.iSendSize = 0;
			break;
		}
	}

	if (user.iSendSize == msg.ph.len)
	{
		user.iSendSize = 0;
	}

	return true;
}

bool HNetworkClient::SendPackets(HNetUser& user)
{
	std::vector<UPACKET>::iterator senditer;
	for (senditer = m_sendPacket.begin(); senditer != m_sendPacket.end(); senditer++)
	{
		{
			if (SendData(user, *senditer) == false)
			{
				g_bConnect = false;
				return false;
			}
		}
	}
	m_sendPacket.clear();

	return true;
}

bool HNetworkClient::MsgEvent(MSG msg)
{
	switch (msg.message)
	{
	case NETWORK_MSG:
		if (WSAGETSELECTERROR(msg.lParam) != 0)
		{
			g_bConnect = false;
			return false;
		}
		WORD dwSelect = WSAGETSELECTEVENT(msg.lParam);
		switch (dwSelect)
		{
		case FD_CONNECT:
		{
			g_bConnect = true;
			SendLoginData(m_Sock, "Lee", "SH");
		}break;
		case FD_CLOSE:
		{
			g_bConnect = false;
		}
		case FD_READ:
		{
			RecvData(m_User);
			// PostMessage(g_hwnd, NETWORK_MSG, m_Sock, FD_READ);
		}break;
		case FD_WRITE:
		{

		}break;
		}
	}

	return true;
}

bool HNetworkClient::InitSocket(std::string ip, int port)
{
	// WSAAsyncSelect 비동기 네트워크
	// 등록한 네트워크 이벤트가 발생하면 윈도우 메세지가 발생하고
	// 윈도우 프로시저가 호출된다.
	int iRet = WSAAsyncSelect(m_Sock, g_hwnd, NETWORK_MSG, 
								FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);

	if (iRet == SOCKET_ERROR)
	{
		return false;
	}

	SOCKADDR_IN sock_in;

	sock_in.sin_family = AF_INET;
	sock_in.sin_addr.s_addr = inet_addr(ip.c_str());
	sock_in.sin_port = htons(port);
	// 전화기 개통
	iRet = connect(m_Sock, (SOCKADDR*)&sock_in, sizeof(sock_in));
	if (iRet == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}

bool HNetworkClient::InitNetwork(std::string ip, int port)
{
	if (HNetwork::InitNetwork(ip.c_str(), port) == false)
	{
		return false;
	}
	//g_bConnect = true;
	//SendLoginData(m_Sock, "ㅇ_ㅇ", "1234");

	return true;
}

void HNetworkClient::SendLoginData(SOCKET sock, std::string id, std::string pw)
{
	HLogin login;
	strcpy_s(login.szID, id.c_str());
	strcpy_s(login.szPW, pw.c_str());
	SendMsg(sock, (char*)&login, sizeof(HLogin), PACKET_LOGIN_REQ);
}

void HNetworkClient::PacketProcess()
{
	std::vector<HPacket>::iterator senditer;
	for (senditer = m_RecvPacket.begin();
		senditer != m_RecvPacket.end();
		senditer++)
	{
		UPACKET* packet = (UPACKET*)&senditer->packet;
		if (packet->ph.type == PACKET_CHAT_MSG)
		{
			HChatMsg* pMsg = (HChatMsg*)&packet->msg;
			printf("\n[%s]%d:%s", pMsg->szName, pMsg->iCnt, pMsg->buffer);
		}
		if (packet->ph.type == PACKET_LOGIN_REQ)
		{
			HLoginResult* ret = (HLoginResult*)packet->msg;
			if (ret->iRet == 1)
			{
				m_bLogin = true;
				// ResumeThread(g_hSendThread);
			}
			else
			{
				SendLoginData(m_Sock, "Lee", "SH");
			}
		}
		// SuspendThread(g_hSendThread);
	}
	m_RecvPacket.clear();
}

bool HNetworkClient::RecvData(HNetUser& user)
{
	if (user.iRecvSize < PACKET_HEADER_SIZE)
	{
		user.iRecvSize += recv(m_Sock, &user.recvBuf[user.iRecvSize],
			PACKET_HEADER_SIZE - user.iRecvSize, 0);

		if (user.iRecvSize == 0 || user.iRecvSize == SOCKET_ERROR)
		{
			user.iRecvSize = 0;
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return false;
			}
			return true;
		}
	}
	else
	{
		UPACKET* packet = (UPACKET*)&user.recvBuf;
		user.iRecvSize += recv(m_Sock, &user.recvBuf[user.iRecvSize],
			packet->ph.len - user.iRecvSize, 0);
		if (user.iRecvSize == 0 || user.iRecvSize == SOCKET_ERROR)
		{
			user.iRecvSize = 0;
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return false;
			}
			return true;
		}
		if (user.iRecvSize == packet->ph.len)
		{
			AddPacket(user, packet);
			memset(user.recvBuf, 0, sizeof(char) * 10000);
			user.iRecvSize = 0;
		}
	}

	return true;
}


HNetworkClient::HNetworkClient()
{
	m_bLogin = false;
}
HNetworkClient::~HNetworkClient()
{

}