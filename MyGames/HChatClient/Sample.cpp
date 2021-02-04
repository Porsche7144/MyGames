#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <conio.h>
#include <string>
#include "HProtocol.h"
#pragma comment (lib, "ws2_32.lib")

static int g_iChatCount = 0;
bool g_bConnect = false;

typedef std::basic_string<TCHAR>		T_STR;
typedef std::basic_string<CHAR>			C_STR;

static char* GetWtM(WCHAR* data)
{
	char retData[4096] = { 0 };
	// 변형되는 문자열의 크기가 반환된다.
	int iLength = WideCharToMultiByte(CP_ACP, 0,
		data, -1, 0, 0, NULL, NULL);
	int iRet = WideCharToMultiByte(CP_ACP, 0,
		data, -1,  //  소스
		retData, iLength, // 대상
		NULL, NULL);
	return retData;
}
static bool GetWtM(WCHAR* src, char* pDest)
{
	// 변형되는 문자열의 크기가 반환된다.
	int iLength = WideCharToMultiByte(CP_ACP, 0,
		src, -1, 0, 0, NULL, NULL);
	int iRet = WideCharToMultiByte(CP_ACP, 0,
		src, -1,  //  소스
		pDest, iLength, // 대상
		NULL, NULL);
	if (iRet == 0) return false;
	return true;
}
static WCHAR* GetMtW(char* data)
{
	WCHAR retData[4096] = { 0 };
	// 변형되는 문자열의 크기가 반환된다.
	int iLength = MultiByteToWideChar(CP_ACP, 0, data, -1, 0, 0);
	int iRet = MultiByteToWideChar(CP_ACP, 0,
		data, -1,  //  소스
		retData, iLength); // 대상
	return retData;
}
static bool GetMtW(char* pSrc, WCHAR* pDest)
{
	// 변형되는 문자열의 크기가 반환된다.
	int iLength = MultiByteToWideChar(CP_ACP, 0,
		pSrc, -1, 0, 0);
	int iRet = MultiByteToWideChar(CP_ACP, 0,
		pSrc, -1,  //  소스
		pDest, iLength); // 대상		
	if (iRet == 0) return false;
	return true;
}

static void Error(const CHAR* msg = 0, const char* lpData = 0)
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

static void Check(int iRet, int line)
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

static void Print(char* fmt, ...) // 나열연산자
{
	va_list arg;
	va_start(arg, fmt);
	char buf[256] = { 0, };
	vsprintf_s(buf, fmt, arg);
	printf("\n=====> %s", buf);
	va_end(arg);
}

int SendMsg(SOCKET sock, char* msg, int iLen, uint16_t type)
{
	UPACKET packet;
	packet.ph.iotype = 0;
	packet.ph.len = PACKET_HEADER_SIZE + iLen;
	packet.ph.type = type;
	packet.ph.time = time(NULL);
	memcpy(&packet.msg, msg, iLen);

	int iSendSize = 0;
	while (iSendSize < packet.ph.len)
	{
		iSendSize += send(sock, (char*)&packet,
			packet.ph.len - iSendSize, 0);
		if (iSendSize == 0 || iSendSize == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
			return 0;
		}
	}
	return iSendSize;
}

DWORD WINAPI SendThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	HChatMsg chatMsg;

	while (g_bConnect)
	{
		printf("\nmsg = ");
		memset(&chatMsg, 0, sizeof(chatMsg));
		fgets(chatMsg.buffer, 128, stdin);
		chatMsg.buffer[strlen(chatMsg.buffer) - 1] = 0;
		chatMsg.iCnt = g_iChatCount++;
		if (SendMsg(sock, (char*)&chatMsg, sizeof(HChatMsg), PACKET_CHAT_MSG) < 0)
		{
			break;
		}
	}
	return 0;
}

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
	linger optLInger;
	int iLingerLen = sizeof(linger);
	optLInger.l_onoff = 1;
	optLInger.l_linger = 1000;
	if (setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&optLInger, iLingerLen) != 0)
	{
		return;
	}
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
	bool g_bConnect = true;

	// 넌블로킹 소켓으로 전환
	unsigned long iMode = 1;
	ioctlsocket(sock, FIONBIO, &iMode);

	// 쓰레드 생성 & 해제
	// 쓰레드를 해제 한다는 것이 쓰레드가 죽는다는 뜻은 아니다.
	DWORD dwID;
	CloseHandle(CreateThread(0, 0, SendThread, (LPVOID)sock, 0, &dwID));


	int iRecvSize = 0;
	char recvBuf[10000] = { 0, };
	int iMsgLength = 0;

	while (1)
	{
		iMsgLength = recv(sock, &recvBuf[iRecvSize], PACKET_HEADER_SIZE - iRecvSize, 0);
		if (iMsgLength == 0 && iMsgLength == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				break;
			}
			continue;
		}
		iRecvSize += iMsgLength;

		if (iRecvSize == PACKET_HEADER_SIZE)
		{
			UPACKET* packet = (UPACKET*)&recvBuf;
			while (iRecvSize < packet->ph.len)
			{
				iRecvSize += recv(sock, &recvBuf[iRecvSize], packet->ph.len - iRecvSize, 0);
			}
			if (packet->ph.type == PACKET_CHAT_MSG)
			{
				HChatMsg* pMsg = (HChatMsg*)&recvBuf[PACKET_HEADER_SIZE];
				printf("\nRecv = %d:%s", pMsg->iCnt, pMsg->buffer);
			}
			if (packet->ph.type == PACKET_USERNAME_REQ)
			{
				char username[64] = "개 똥 이";
				SendMsg(sock, username, 64, PACKET_USERNAME_ACK);
			}
			memset(recvBuf, 0, sizeof(char*) * 10000);
			iRecvSize = 0;
		}
	}
	g_bConnect = false;
	shutdown(sock, SD_SEND);
	closesocket(sock);
	WSACleanup();
}
