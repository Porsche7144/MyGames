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
	// 2.2버전
	WSADATA wsa;

	// 사용 후 WSACleanup로 종료.
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	int iRet;

	// 주소체계
	// 소켓타입(TCP - SOCK_STREAM, UDP - SOCK_DGRAM)
	// 프로토콜(IPPROTO_TCP,    IPPROTO_UDP  , 0을 입력하면 소켓타입에 맞춰서)

	//사용 후 closesocket로 종료.
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	// getsockopt 소켓정보를 얻어오는 함수
	int sockType;
	int sockTypeLen = sizeof(int);
	getsockopt(sock, SOL_SOCKET, SO_TYPE, (char*)&sockType, &sockTypeLen);
	if (sockType == SOCK_STREAM)
	{
		printf("%s\r\m", "TCP연결");
	}
	else
	{
		printf("%s\r\m", "UDP연결");
	}

	// 송수신 버퍼의 크기
	int iSendBuffer = 100000;
	// setsockopt 소켓 옵션을 직접 설정.
	// 기본적으로 윈도우에서 버퍼를 제공하지만 버퍼의 크기 변경도 가능하다.
	setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&iSendBuffer, sockTypeLen);
	
	// setsockopt로 값 변경 후 getsokopt로 변경된 값 확인.
	// SO_SNDBUF send값
	getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&iSendBuffer, &sockTypeLen);
	printf("SendBuffer = %d\n", sockType);
	// SO_RCVBUF recive값
	getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&iSendBuffer, &sockTypeLen);
	printf("RecvBuffer = %d\n", sockType);

	int optval = 1;

	// SO_EXCLUSIVEADDRUSE
	// 1개의 프로세스에서만 IP와 Port 재사용 가능하다.
	// 여러개 실행시 bind 함수가 오류를 반환한다.

	Check(setsockopt(sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&optval, sizeof(optval)), __LINE__);


	// SO_REUSEADDR
	// 다수의 프로세스에서 IP와 Port 재사용이 가능하다.
	//iRet = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));
	//if (iRet == SOCKET_ERROR)
	//{
	//	return;
	//}

	// Linger
	// 소켓을 닫을 때 남은 데이터의 처리 규칙 지정
	// close()는 바로 리턴하며, 버퍼에 있는 데이터는 버림. 
	// l_onoff > 0, l_linger == 0 TCP 연결 상태에서는 상대편 호스트에게 리셋을 위한 RST 패킷 전송
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

	// 클라로부터 받기, 전화기 생성
	Check(bind(sock, (SOCKADDR*)&sock_in, sizeof(sock_in)), __LINE__);
	// 전화기 개통
	Check(listen(sock, SOMAXCONN), __LINE__);

	bool bConnect = false;
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);

	while (1)
	{
		// 클라이언트의 주소를 받아온다.
		SOCKET client = accept(sock, (SOCKADDR*)&clientAddr, &len);
		if (client == INVALID_SOCKET)
		{
			break;
		}

		// inet_ntoa 네트워크 주소 변환
		// ntohs 네트워크에서 호스트로
		// htons 호스트에서 네트워크로
		printf("\n접속 : %s , %d", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

		int iCount = 0;
		bConnect = true;

		HMsg msg;

		char recvBuf[10000] = { 0, };
		int iRecvSize = 0;
		int iSendSize = 0;
		int iPacketSize = sizeof(HMsg);

		// 1초
		while (bConnect)
		{

			while (iRecvSize < iPacketSize)
			{
				// 메세지 이어서 받기
				iRecvSize += recv(client, recvBuf, iPacketSize - iRecvSize, 0);
				// 받을 때
				if (iRecvSize == 0 || iRecvSize == SOCKET_ERROR)
				{
					bConnect = false;
					break;
				}
			}

			// 보낼 때
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

		// shutdown  closesocket과 같은 종료명령.
		// 차이점은 두번째 매개변수인 how_to에 어떤 값을 넣느냐에 따라
		// recv와 send 차단여부를 선택할 수 있다.
		shutdown(client, SD_SEND);
		closesocket(client);
		printf("\n해제 : %s , %d", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
	}

	closesocket(sock);
	WSACleanup();

}