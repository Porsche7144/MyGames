#include "HAcceptor.h"
#include "HIocp.h"
#include "HSessionMgr.h"

bool HAcceptor::InitNetwork(std::string ip, int port)
{
	int iRet;
	m_Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_Sock == INVALID_SOCKET)
	{
		Error("socket");
		return false;
	}
	// ���� Ÿ�� Ȯ��
	int sockType;
	int sockTypeLen = sizeof(int);
	getsockopt(m_Sock, SOL_SOCKET, SO_TYPE,
		(char*)&sockType, &sockTypeLen);
	if (sockType == SOCK_STREAM)
		printf("%s\r\n", "SOCK_STREAM.");
	else
		printf("%s\r\n", "SOCK_DGRAM");

	//�ۼ��� ���� ũ�� Ȯ��	
	int iSendBuffer = 100000;
	setsockopt(m_Sock, SOL_SOCKET, SO_SNDBUF,
		(char*)&iSendBuffer, sockTypeLen);

	getsockopt(m_Sock, SOL_SOCKET, SO_SNDBUF,
		(char*)&sockType, &sockTypeLen);
	printf("SendBuffer=%d\n", sockType);
	getsockopt(m_Sock, SOL_SOCKET, SO_RCVBUF,
		(char*)&sockType, &sockTypeLen);
	printf("RecvBuffer=%d\n", sockType);


	int optval = 1;
	// 1���� ���μ��������� ip&port ���� ����
	// bind�Լ����� ������ ��´�.
	Check(
		setsockopt(m_Sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE,
		(char*)&optval, sizeof(optval)),
		__LINE__
	);
	// �ټ� ���μ������� ip&port ���� ����
	/*iRet = setsockopt(m_Sock, SOL_SOCKET, SO_REUSEADDR,
		(char*)&optval, sizeof(optval));
	if (iRet == SOCKET_ERROR)
	{
		return false;
	}*/

	linger optLinger;
	int iLingerLen = sizeof(optLinger);
	optLinger.l_onoff = 1;
	optLinger.l_linger = 1000;
	if (setsockopt(m_Sock, SOL_SOCKET, SO_LINGER,
		(char*)&optLinger, iLingerLen) != 0)
	{
		Error("SO_LINGER");
		return false;
	}

	if (InitSocket(ip, port) == false)
	{
		return false;
	}
	return true;
}
bool HAcceptor::InitSocket(std::string ip, int port)
{
	// �ͺ��ŷ �������� ��ȯ
	unsigned long iMode = 1;
	ioctlsocket(m_Sock, FIONBIO, &iMode);

	SOCKADDR_IN sa;
	// ����Ʈ ���� ���� 	
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr("192.168.0.151");
	//error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS
	sa.sin_port = htons(port);

	//SO_REUSEADDR�� ������ ������ ���� ���� ������ �ȴ�.
	Check(
		bind(m_Sock, (SOCKADDR*)&sa, sizeof(sa)),
		__LINE__
	);
	int iRet = listen(m_Sock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)
	{
		Error("listen");
		return false;
	}
	return true;
}
bool HAcceptor::CloseSocket()
{
	closesocket(m_Sock);
	return true;
}
bool HAcceptor::DeleteNetwork()
{
	CloseSocket();
	return true;
}
bool HAcceptor::Run()
{
	while (m_bStarted)
	{
		{
			HLock lock(this);
			if (Accept() == false) break;
		}
		Sleep(1);
	}
	return false;
}
bool HAcceptor::Accept()
{
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);

	SOCKET client = accept(m_Sock, (SOCKADDR*)&clientAddr, &len);
	if (client == INVALID_SOCKET)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
	}
	else
	{
		AddSession(client, clientAddr);
	}
	return true;
}
bool HAcceptor::AddSession(SOCKET sock, SOCKADDR_IN SockAddr)
{
	{
		HLock lock(this);
		HNetUser* user = new HNetUser;
		user->m_Sock = sock;
		user->m_addr = SockAddr;
		I_Sesstion.AddUser(user);

		I_IOCP.SetBind(sock, (ULONG_PTR)user);

		user->WaitRecieve();

		printf("\n����->%s:%d",
			inet_ntoa(SockAddr.sin_addr),
			ntohs(SockAddr.sin_port));
	}
	return true;
}
HAcceptor::HAcceptor()
{
	// 2.2 ver
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
}
HAcceptor::~HAcceptor()
{
	WSACleanup();
}
