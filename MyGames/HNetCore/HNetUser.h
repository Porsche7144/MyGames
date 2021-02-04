#pragma once
#include "HServerObj.h"

#define g_iRecvBufferSize 512
#define g_iMaxDataBufferSize (g_iRecvBufferSize * 3)

class HNetUser;
struct HPacket
{
	HNetUser* pUser;
	UPACKET	  packet;
};

struct OVERLAPPED2 : OVERLAPPED
{
	enum { RECV_MODE = 1, SEND_MODE = 2, EXIT_MODE};
	int iType; // 타입이 1이면 recv , 2면 send
};

class HNetUser
{
public:
	SOCKET		m_Sock;
	SOCKADDR_IN m_addr;
	char	m_szRecvBuf[g_iRecvBufferSize];
	char	m_szSendBuf[g_iRecvBufferSize];

	// WSABUF 윈속 구조체
	// 소켓 데이터의 송수신에서 데이터를 저장하고 보내기 위해 사용하는 버퍼
	WSABUF		m_wsaRecvBuf;
	WSABUF		m_wsaSendBuf;

	OVERLAPPED2 m_ovRecv;
	OVERLAPPED2 m_ovSend;

	bool		m_bExit;
	T_STR		m_szName;
	std::vector<UPACKET>	m_SendPacket;

public:
	char	m_szDataBuf[g_iMaxDataBufferSize];
	int		m_iPacketPos; // 패킷 시작주소
	int		m_iReadPos;   // 패킷 데이터 저장 주소
	int		m_iWritePos;  // 패킷 데이터 저장 주소

public:
	virtual bool DispatchRead(DWORD dwTrans, OVERLAPPED2* ov);
	virtual bool DispatchWrite(DWORD dwTrans, OVERLAPPED2* ov);

public:
	virtual bool WaitRecieve();
	HNetUser();
	virtual ~HNetUser();

};