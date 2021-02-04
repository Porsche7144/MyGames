#include "HNetworkClient.h"

bool HNetworkClientEventSelect::InitSocket(std::string ip, int port)
{
	// �̺�Ʈ ����
	m_EventArray[m_iArrayCount] = WSACreateEvent();
	// WSAEventSelect �̺�Ʈ ��ü. �̺�Ʈ�� ��ȯ�޾Ƽ� ó��.
	WSAEventSelect(m_Sock, m_EventArray[m_iArrayCount], FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
	m_iArrayCount++;	

	// �ͺ��ŷ �������� ��ȯ
	unsigned long iMode = 0;
	ioctlsocket(m_Sock, FIONBIO, &iMode);

	SOCKADDR_IN sock_in;

	sock_in.sin_family = AF_INET;
	sock_in.sin_addr.s_addr = inet_addr(ip.c_str());
	sock_in.sin_port = htons(port);
	// ��ȭ�� ����
	int iRet = connect(m_Sock, (SOCKADDR*)&sock_in, sizeof(sock_in));
	if (iRet == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}

bool HNetworkClientEventSelect::InitNetwork(std::string ip, int port)
{
	if (HNetwork::InitNetwork(ip.c_str(), port) == false)
	{
		return false;
	}

	return true;
}

bool HNetworkClientEventSelect::Frame()
{
	// WSAWaitForMultipleEvents �̺�Ʈ ��ü�� ���� Ȥ�� �ϳ��� ��ȣ ����,
	// ���� �ð� Ȥ�� �Ϸ� ��ƾ�� ����� ��ȯ
	int iIndex = WSAWaitForMultipleEvents(m_iArrayCount, m_EventArray, FALSE, 0, FALSE);

	// ����üũ
	if (iIndex == WSA_WAIT_FAILED) return false;
	if (iIndex == WSA_WAIT_TIMEOUT) return false;
	iIndex -= WSA_WAIT_EVENT_0;

	WSANETWORKEVENTS netEvent;

	for (int iEvent = iIndex; iEvent < m_iArrayCount; iEvent++)
	{
		int iSignal = WSAWaitForMultipleEvents(1, &m_EventArray[iEvent], TRUE, 0, FALSE);
		if (iSignal == WSA_WAIT_FAILED)
		{
			HNetwork::g_bConnect = false;
			break;
		}
		if (iSignal == WSA_WAIT_TIMEOUT)
		{
			continue;
		}

		int iRet = WSAEnumNetworkEvents(m_Sock, m_EventArray[iEvent], &netEvent);
		if (iRet == SOCKET_ERROR)
		{
			Check(iRet, __LINE__);
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return false;
			}
			break;
		}

		if (netEvent.lNetworkEvents & FD_CONNECT)
		{
			if (netEvent.iErrorCode[FD_CONNECT_BIT] != 0)
			{
				HNetwork::g_bConnect = false;
				continue;
			}
			HNetwork::g_bConnect = true;
			SendLoginData(m_Sock, "Lee", "SH");
		}

		if (netEvent.lNetworkEvents & FD_READ)
		{
			if (netEvent.iErrorCode[FD_READ_BIT] != 0)
			{
				HNetwork::g_bConnect = false;
				continue;
			}
			if (RecvData(m_User) == false)
			{
				HNetwork::g_bConnect = false;
				continue;
			}
		}

		if (netEvent.lNetworkEvents & FD_WRITE)
		{
			if (netEvent.iErrorCode[FD_WRITE_BIT] != 0)
			{
				HNetwork::g_bConnect = false;
				continue;
			}
		}

		if (netEvent.lNetworkEvents & FD_CLOSE)
		{
			if (netEvent.iErrorCode[FD_CLOSE_BIT] != 0)
			{
				HNetwork::g_bConnect = false;
				continue;
			}
		}
	}

	return true;
}

