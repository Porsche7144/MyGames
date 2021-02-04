#include "HNetworkClient.h"

static int g_iChatCount = 0;
bool g_bConnect = false;

DWORD WINAPI SendThread(LPVOID arg)
{
	HNetwork* net = (HNetwork*)arg;
	HChatMsg chatMsg;

	while (g_bConnect)
	{
		memset(&chatMsg, 0, sizeof(chatMsg));
		// fgets(chatMsg.buffer, 128, stdin);
		strcpy_s(chatMsg.szName, "ㅇ_ㅇ");
		strcpy_s(chatMsg.buffer, "1234");
		chatMsg.buffer[strlen(chatMsg.buffer) - 1] = 0;
		chatMsg.iCnt = g_iChatCount++;
		EnterCriticalSection(&net->m_cs);
		if (net->SendMsg(net->m_Sock, (char*)&chatMsg, sizeof(HChatMsg), PACKET_CHAT_MSG) < 0)
		{
			LeaveCriticalSection(&net->m_cs);
			break;
		}
	}
	LeaveCriticalSection(&net->m_cs);
	Sleep(1000);
	return 0;
}

bool HNetworkClient::InitSocket(std::string ip, int port)
{
	SOCKADDR_IN sock_in;

	sock_in.sin_family = AF_INET;
	sock_in.sin_addr.s_addr = inet_addr(ip.c_str());
	sock_in.sin_port = htons(port);
	// 전화기 개통
	int iRet = connect(m_Sock, (SOCKADDR*)&sock_in, sizeof(sock_in));
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
	HNetwork::g_bConnect = true;

	SendLoginData(m_Sock, "Lee", "SH");

	// 쓰레드 생성
	DWORD dwID;
	// CREATE_SUSPENDED 쓰레드 생성 후 작업하지 않고 대기상태.
	g_hSendThread = CreateThread(0, 0, SendThread, (LPVOID)this, 0,  //CREATE_SUSPENDED, 
								&dwID);
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
				ResumeThread(g_hSendThread);
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
		user.iRecvSize += recv(user.m_Sock, &user.recvBuf[user.iRecvSize],
			PACKET_HEADER_SIZE - user.iRecvSize, 0);

		if (user.iRecvSize == 0 || user.iRecvSize == SOCKET_ERROR)
		{
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
		user.iRecvSize += recv(user.m_Sock, &user.recvBuf[user.iRecvSize],
			packet->ph.len - user.iRecvSize, 0);
		if (user.iRecvSize == 0 || user.iRecvSize == SOCKET_ERROR)
		{
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

}
HNetworkClient::~HNetworkClient()
{

}