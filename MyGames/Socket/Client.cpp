#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <time.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")


struct HMsg
{
	int iCnt;
	char Buffer[3000];
};

void main(int argc, char* argv[])
{
	// 2.2����
	WSADATA wsa;

	// ��� �� WSACleanup�� ����.
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}

	while (1)
	{
		// �ּ�ü��
		// ����Ÿ��(TCP - SOCK_STREAM, UDP - SOCK_DGRAM)
		// ��������(IPPROTO_TCP,    IPPROTO_UDP  , 0�� �Է��ϸ� ����Ÿ�Կ� ���缭)

		const char* ipAddr = "192.168.0.154"; // argv[1];
		unsigned short iPort = 10000;

		//��� �� closesocket�� ����.
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		int optval = 1;
		// setsockopt  ���������� �ĺ��ϴ� �Լ�.
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)) != 0)
		{
			break;
		}


		// Linger
		// ������ ���� �� ���� �������� ó�� ��Ģ ����
		// close()�� �ٷ� �����ϸ�, ���ۿ� �ִ� �����ʹ� ����. 
		// l_onoff > 0, l_linger == 0 TCP ���� ���¿����� ����� ȣ��Ʈ���� ������ ���� RST ��Ŷ ����
		/*linger optLInger;
		int iLingerLen = sizeof(linger);
		optLInger.l_onoff = 1;
		optLInger.l_linger = 1000;
		if (setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&optLInger, iLingerLen) != 0)
		{
			break;
		}*/
		SOCKADDR_IN sock_in;

		// inet_ntoa ��Ʈ��ũ �ּ� ��ȯ
		// ntohs ��Ʈ��ũ���� ȣ��Ʈ��
		// htons ȣ��Ʈ���� ��Ʈ��ũ��
		sock_in.sin_family = AF_INET;
		sock_in.sin_addr.s_addr = inet_addr(ipAddr);
		sock_in.sin_port = htons(iPort);

		// ������ ����
		int iRet = connect(sock, (SOCKADDR*)&sock_in, sizeof(sock_in));
		if (iRet == SOCKET_ERROR)
		{
			return;
		}

		int iCount = 0;
		clock_t start = clock();
		clock_t end = clock();
		HMsg msg;

		int iRecvSize = 0;
		int iSendSIze = 0;
		int iPacketSize = sizeof(HMsg);
		char recvBuf[10000] = { 0, };

		bool bConnect = true;
		// 1��
		while (end - start < 1000 && bConnect)
		{
			while (iSendSIze < iPacketSize)
			{
				memset(&msg, 0, sizeof(msg));
				strcpy_s(msg.Buffer, 32, "�ȳ��ϼ���");
				msg.iCnt = iCount;
				char recvBuffer[3001] = { 0, };
				int iLen = sizeof(msg);
				clock_t t1 = clock();

				// ���� ��
				iSendSIze += send(sock, (char*)&msg, iPacketSize - iSendSIze, 0);
				if (iSendSIze == 0 || iSendSIze == SOCKET_ERROR)
				{
					bConnect = false;
					break;
				}
			}

			// ���� ��
			memset(recvBuf, 0, sizeof(char) * 10000);
			while (iRecvSize < iPacketSize && bConnect)
			{
				iRecvSize = recv(sock, recvBuf, iPacketSize - iRecvSize, 0);
				if (iRecvSize == 0 || iRecvSize == SOCKET_ERROR)
				{
					bConnect = false;
					break;
				}
				if (sizeof(HMsg) == iRecvSize)
				{
					memcpy(&msg, recvBuf, sizeof(HMsg));
					printf("\n%d : %s", msg.iCnt, msg.Buffer);
				}
				
			}
			iSendSIze = 0;
			iRecvSize = 0;
			end = clock();
			iCount++;
		}

		closesocket(sock);
		Sleep(1000);

	}
	WSACleanup();

}