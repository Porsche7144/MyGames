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
	// 2.2버전
	WSADATA wsa;

	// 사용 후 WSACleanup로 종료.
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}

	while (1)
	{
		// 주소체계
		// 소켓타입(TCP - SOCK_STREAM, UDP - SOCK_DGRAM)
		// 프로토콜(IPPROTO_TCP,    IPPROTO_UDP  , 0을 입력하면 소켓타입에 맞춰서)

		const char* ipAddr = "192.168.0.154"; // argv[1];
		unsigned short iPort = 10000;

		//사용 후 closesocket로 종료.
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		int optval = 1;
		// setsockopt  소켓정보를 식별하는 함수.
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)) != 0)
		{
			break;
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

		int iCount = 0;
		clock_t start = clock();
		clock_t end = clock();
		HMsg msg;

		int iRecvSize = 0;
		int iSendSIze = 0;
		int iPacketSize = sizeof(HMsg);
		char recvBuf[10000] = { 0, };

		bool bConnect = true;
		// 1초
		while (end - start < 1000 && bConnect)
		{
			while (iSendSIze < iPacketSize)
			{
				memset(&msg, 0, sizeof(msg));
				strcpy_s(msg.Buffer, 32, "안녕하세요");
				msg.iCnt = iCount;
				char recvBuffer[3001] = { 0, };
				int iLen = sizeof(msg);
				clock_t t1 = clock();

				// 보낼 때
				iSendSIze += send(sock, (char*)&msg, iPacketSize - iSendSIze, 0);
				if (iSendSIze == 0 || iSendSIze == SOCKET_ERROR)
				{
					bConnect = false;
					break;
				}
			}

			// 받을 때
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