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
	// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
	int iLength = WideCharToMultiByte(CP_ACP, 0,
		data, -1, 0, 0, NULL, NULL);
	int iRet = WideCharToMultiByte(CP_ACP, 0,
		data, -1,  //  �ҽ�
		retData, iLength, // ���
		NULL, NULL);
	return retData;
}
static bool GetWtM(WCHAR* src, char* pDest)
{
	// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
	int iLength = WideCharToMultiByte(CP_ACP, 0,
		src, -1, 0, 0, NULL, NULL);
	int iRet = WideCharToMultiByte(CP_ACP, 0,
		src, -1,  //  �ҽ�
		pDest, iLength, // ���
		NULL, NULL);
	if (iRet == 0) return false;
	return true;
}
static WCHAR* GetMtW(char* data)
{
	WCHAR retData[4096] = { 0 };
	// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
	int iLength = MultiByteToWideChar(CP_ACP, 0, data, -1, 0, 0);
	int iRet = MultiByteToWideChar(CP_ACP, 0,
		data, -1,  //  �ҽ�
		retData, iLength); // ���
	return retData;
}
static bool GetMtW(char* pSrc, WCHAR* pDest)
{
	// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
	int iLength = MultiByteToWideChar(CP_ACP, 0,
		pSrc, -1, 0, 0);
	int iRet = MultiByteToWideChar(CP_ACP, 0,
		pSrc, -1,  //  �ҽ�
		pDest, iLength); // ���		
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

static void Print(char* fmt, ...) // ����������
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
		printf("\n����� xxx.exe [ip] [port]");
		printf("\n����� xxx.exe 192.168.0.151 10000");
		return;
	}*/

	// 2.2����
	WSADATA wsa;

	// ��� �� WSACleanup�� ����.
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	const char* ipAddr = "192.168.0.154"; // argv[1];
	unsigned short iPort = 10000; // atoi(argv[2]);

	// �ּ�ü��
	// ����Ÿ��(TCP - SOCK_STREAM, UDP - SOCK_DGRAM)
	// ��������(IPPROTO_TCP,    IPPROTO_UDP  , 0�� �Է��ϸ� ����Ÿ�Կ� ���缭)

	//��� �� closesocket�� ����.
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	int optval = 1;
	// setsockopt  ���������� �ĺ��ϴ� �Լ�.

	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)) != 0)
	{
		return;
	}


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
		return;
	}
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
	bool g_bConnect = true;

	// �ͺ��ŷ �������� ��ȯ
	unsigned long iMode = 1;
	ioctlsocket(sock, FIONBIO, &iMode);

	// ������ ���� & ����
	// �����带 ���� �Ѵٴ� ���� �����尡 �״´ٴ� ���� �ƴϴ�.
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
				char username[64] = "�� �� ��";
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
