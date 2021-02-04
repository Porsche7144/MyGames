#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <conio.h>
#include <string>
#pragma comment (lib, "ws2_32.lib")


struct TMsg
{
	int  iCnt;
	char buffer[3000];
};
void main(int argc, char* argv[])
{
	/*if (argc != 3)
	{
		printf("\n사용방법 xxx.exe [ip] [port]");
		printf("\n사용방법 xxx.exe 192.168.0.151 10000");
		return;
	}*/

	// 2.2버전
	WSADATA wsa;

	// 사용 후 WSACleanup로 종료.
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	const char* ipAddr = "192.168.0.154"; // argv[1];
	unsigned short iPort = 10000; // atoi(argv[2]);

	// 주소체계
	// 소켓타입(TCP - SOCK_STREAM, UDP - SOCK_DGRAM)
	// 프로토콜(IPPROTO_TCP,    IPPROTO_UDP  , 0을 입력하면 소켓타입에 맞춰서)

	//사용 후 closesocket로 종료.
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	int optval = 1;
	// setsockopt  소켓정보를 식별하는 함수.
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)) != 0)
	{
		return;
	}


	// Linger
	// 소켓을 닫을 때 남은 데이터의 처리 규칙 지정
	// close()는 바로 리턴하며, 버퍼에 있는 데이터는 버림. 
	// l_onoff > 0, l_linger == 0 TCP 연결 상태에서는 상대편 호스트에게 리셋을 위한 RST 패킷 전송
	/*linger optLInger;
	int iLingerLen = sizeof(linger);
	optLInger.l_onoff = 1;
	optLInger.l_linger = 1000;
	if (setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&optLInger, iLingerLen) != 0)
	{
		break;
	}*/
	SOCKADDR_IN sock_in;

	// inet_ntoa 네트워크 주소 변환
	// ntohs 네트워크에서 호스트로
	// htons 호스트에서 네트워크로
	sock_in.sin_family = AF_INET;
	sock_in.sin_addr.s_addr = inet_addr(ipAddr);
	sock_in.sin_port = htons(iPort);

	// 서버로 연결
	int iRet = connect(sock, (SOCKADDR*)&sock_in, sizeof(sock_in));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}

	// 넌블로킹 소켓으로 전환
	unsigned long iMode = 1;
	ioctlsocket(sock, FIONBIO, &iMode);

	int iCount = 0;
	TMsg msg;
	int iRecvSize = 0;
	int iSendSize = 0;
	int iPacketSize = sizeof(TMsg);
	char recvBuf[10000] = { 0, };
	char sendBuf[10000] = { 0, };

	bool bConnect = true;
	bool bRecvData = false;
	int iMsgLength = 0;
	while (bConnect)
	{
		if (_kbhit() != 0)
		{
			int iValue = _getche();
			int ilen = strlen(sendBuf);
			if (ilen == 0 && iValue == '\r')
			{
				break;
			}
			if (iValue == '\r')
			{
				memset(&msg, 0, sizeof(msg));
				strcpy_s(msg.buffer, sendBuf);
				msg.iCnt = iCount++;

				iMsgLength = 0;
				while (iSendSize < iPacketSize)
				{
					char recvBuf[3001] = { 0, };
					iSendSize += send(sock, (char*)&msg,
						iPacketSize - iSendSize, 0);
					if (iSendSize == 0 || iSendSize == SOCKET_ERROR)
					{
						if (WSAGetLastError() != WSAEWOULDBLOCK)
						{
							bConnect = false;
						}
						break;
					}
				}
				memset(sendBuf, 0, sizeof(char) * 10000);
			}
			else
			{
				sendBuf[iMsgLength++] = iValue;
			}
		}
		memset(recvBuf, 0, sizeof(char) * 10000);
		bRecvData = true;
		while (iRecvSize < iPacketSize && bRecvData)
		{
			iRecvSize += recv(sock, recvBuf,
				iPacketSize - iRecvSize, 0);
			if (iRecvSize == 0)
			{
				bConnect = false;
				break;
			}
			if (iRecvSize == SOCKET_ERROR)
			{
				bRecvData = false;
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					bConnect = false;
					break;
				}
			}
			if (sizeof(TMsg) == iRecvSize)
			{
				memcpy(&msg, recvBuf, sizeof(TMsg));
				printf("\nRecv = %d:%s\n",
					msg.iCnt, msg.buffer);
				bRecvData = false;
			}
		}
		iSendSize = 0;
		iRecvSize = 0;
	}
	shutdown(sock, SD_SEND);
	closesocket(sock);
	WSACleanup();
}
