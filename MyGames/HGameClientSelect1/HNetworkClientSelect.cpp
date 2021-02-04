#include "HNetworkClient.h"

bool HNetworkClientSelect::InitSocket(std::string ip, int port)
{
	// �ͺ��ŷ �������� ��ȯ
	unsigned long iMode = 1;
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

bool HNetworkClientSelect::InitNetwork(std::string ip, int port)
{
	if (HNetwork::InitNetwork(ip.c_str(), port) == false)
	{
		return false;
	}
	HNetwork::g_bConnect = true;
	SendLoginData(m_Sock, "Lee", "SH");

	return true;
}

bool HNetworkClientSelect::Frame()
{
	if (!HNetwork::g_bConnect) return true;

	// �ʱ�ȭ
	FD_ZERO(&m_rSet);
	FD_ZERO(&m_wSet);
	FD_ZERO(&m_eSet);
	
	FD_SET(m_Sock, &m_rSet);
	FD_SET(m_Sock, &m_wSet);
	FD_SET(m_Sock, &m_eSet);

	timeval time;
	time.tv_sec = 5; // ��
	time.tv_usec = 0; // ����ũ����  1/1000000

	int iRet = select(0, &m_rSet, &m_wSet, &m_eSet, &time);

	// ����üũ
	if (iRet == SOCKET_ERROR) return false;
	if (iRet == 0) return true; // Ÿ�Ӿƿ�

	if (FD_ISSET(m_Sock, &m_eSet))
	{
		HNetwork::g_bConnect = false;
		return true;
	}

	if (FD_ISSET(m_Sock, &m_rSet))
	{
		if (RecvData(m_User) == false)
		{
			HNetwork::g_bConnect = false;
			return true;
		}
	}

	if (FD_ISSET(m_Sock, &m_wSet))
	{
		return true;
	}

	return true;
}