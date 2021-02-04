#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <time.h>
#include <stdio.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")


struct HMsg
{
	int iCnt;
	char Buffer[3000];
};

void Error(const CHAR* msg = 0, const char* lpData = 0)
{
	LPVOID* lpMsg = 0;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(CHAR*)&lpMsg, 0, NULL);

	std::string szBuffer = (lpData != nullptr) ? lpData : "";
	szBuffer += "\n";
	szBuffer += (CHAR*)lpMsg;

	MessageBoxA(NULL, szBuffer.c_str(), msg, MB_ICONERROR);
	LocalFree(lpMsg);
}

void Check(int iRet, int line)
{
	if (iRet == SOCKET_ERROR)
	{
		CHAR	szBuffer[256] = { 0, };
		sprintf_s(szBuffer,
			"%s[%d]",
			__FILE__,
			line);
		Error("ERROR", szBuffer);
		exit(1);
	}
}

void main()
{
	// 2.2����
	WSADATA wsa;

	// ��� �� WSACleanup�� ����.
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	int iRet;

	// �ּ�ü��
	// ����Ÿ��(TCP - SOCK_STREAM, UDP - SOCK_DGRAM)
	// ��������(IPPROTO_TCP,    IPPROTO_UDP  , 0�� �Է��ϸ� ����Ÿ�Կ� ���缭)

	//��� �� closesocket�� ����.
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	// getsockopt ���������� ������ �Լ�
	int sockType;
	int sockTypeLen = sizeof(int);
	getsockopt(sock, SOL_SOCKET, SO_TYPE, (char*)&sockType, &sockTypeLen);
	if (sockType == SOCK_STREAM)
	{
		printf("%s\r\m", "TCP����");
	}
	else
	{
		printf("%s\r\m", "UDP����");
	}

	// �ۼ��� ������ ũ��
	int iSendBuffer = 100000;
	// setsockopt ���� �ɼ��� ���� ����.
	// �⺻������ �����쿡�� ���۸� ���������� ������ ũ�� ���浵 �����ϴ�.
	setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&iSendBuffer, sockTypeLen);
	
	// setsockopt�� �� ���� �� getsokopt�� ����� �� Ȯ��.
	// SO_SNDBUF send��
	getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&iSendBuffer, &sockTypeLen);
	printf("SendBuffer = %d\n", sockType);
	// SO_RCVBUF recive��
	getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&iSendBuffer, &sockTypeLen);
	printf("RecvBuffer = %d\n", sockType);

	int optval = 1;

	// SO_EXCLUSIVEADDRUSE
	// 1���� ���μ��������� IP�� Port ���� �����ϴ�.
	// ������ ����� bind �Լ��� ������ ��ȯ�Ѵ�.

	Check(setsockopt(sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&optval, sizeof(optval)), __LINE__);


	// SO_REUSEADDR
	// �ټ��� ���μ������� IP�� Port ������ �����ϴ�.
	//iRet = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));
	//if (iRet == SOCKET_ERROR)
	//{
	//	return;
	//}

	// Linger
	// ������ ���� �� ���� �������� ó�� ��Ģ ����
	// close()�� �ٷ� �����ϸ�, ���ۿ� �ִ� �����ʹ� ����. 
	// l_onoff > 0, l_linger == 0 TCP ���� ���¿����� ����� ȣ��Ʈ���� ������ ���� RST ��Ŷ ����
	linger optLInger;
	int iLingerLen = sizeof(linger);
	optLInger.l_onoff = 1;
	optLInger.l_linger = 1000;
	if (setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&optLInger, iLingerLen) != 0)
	{
		Error("SO_LINGER");
		return;
	}


	SOCKADDR_IN sock_in;

	sock_in.sin_family = AF_INET;
	sock_in.sin_addr.s_addr = inet_addr("192.168.0.154");
	sock_in.sin_port = htons(10000);

	// Ŭ��κ��� �ޱ�, ��ȭ�� ����
	Check(bind(sock, (SOCKADDR*)&sock_in, sizeof(sock_in)), __LINE__);
	// ��ȭ�� ����
	Check(listen(sock, SOMAXCONN), __LINE__);

	bool bConnect = false;
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);

	while (1)
	{
		// Ŭ���̾�Ʈ�� �ּҸ� �޾ƿ´�.
		SOCKET client = accept(sock, (SOCKADDR*)&clientAddr, &len);
		if (client == INVALID_SOCKET)
		{
			break;
		}

		// inet_ntoa ��Ʈ��ũ �ּ� ��ȯ
		// ntohs ��Ʈ��ũ���� ȣ��Ʈ��
		// htons ȣ��Ʈ���� ��Ʈ��ũ��
		printf("\n���� : %s , %d", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

		int iCount = 0;
		bConnect = true;

		HMsg msg;

		char recvBuf[10000] = { 0, };
		int iRecvSize = 0;
		int iSendSize = 0;
		int iPacketSize = sizeof(HMsg);

		// 1��
		while (bConnect)
		{

			while (iRecvSize < iPacketSize)
			{
				// �޼��� �̾ �ޱ�
				iRecvSize += recv(client, recvBuf, iPacketSize - iRecvSize, 0);
				// ���� ��
				if (iRecvSize == 0 || iRecvSize == SOCKET_ERROR)
				{
					bConnect = false;
					break;
				}
			}

			// ���� ��
			while (iSendSize < iPacketSize && bConnect)
			{
				memcpy(&msg, recvBuf, sizeof(HMsg));
				iSendSize = send(client, (char*)&msg, iPacketSize - iSendSize, 0);
				if (iSendSize == 0 || iSendSize == SOCKET_ERROR)
				{
					bConnect = false;
					break;
				}
			}
			iSendSize = 0;
			iRecvSize = 0;
			printf("\n%d - %s", msg.iCnt, msg.Buffer);
		}

		// shutdown  closesocket�� ���� ������.
		// �������� �ι�° �Ű������� how_to�� � ���� �ִ��Ŀ� ����
		// recv�� send ���ܿ��θ� ������ �� �ִ�.
		shutdown(client, SD_SEND);
		closesocket(client);
		printf("\n���� : %s , %d", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
	}

	closesocket(sock);
	WSACleanup();

}