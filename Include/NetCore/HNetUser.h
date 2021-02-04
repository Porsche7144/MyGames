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
	int iType; // Ÿ���� 1�̸� recv , 2�� send
};

class HNetUser
{
public:
	SOCKET		m_Sock;
	SOCKADDR_IN m_addr;
	char	m_szRecvBuf[g_iRecvBufferSize];
	char	m_szSendBuf[g_iRecvBufferSize];

	// WSABUF ���� ����ü
	// ���� �������� �ۼ��ſ��� �����͸� �����ϰ� ������ ���� ����ϴ� ����
	WSABUF		m_wsaRecvBuf;
	WSABUF		m_wsaSendBuf;

	OVERLAPPED2 m_ovRecv;
	OVERLAPPED2 m_ovSend;

	bool		m_bExit;
	T_STR		m_szName;
	std::vector<UPACKET>	m_SendPacket;

public:
	char	m_szDataBuf[g_iMaxDataBufferSize];
	int		m_iPacketPos; // ��Ŷ �����ּ�
	int		m_iReadPos;   // ��Ŷ ������ ���� �ּ�
	int		m_iWritePos;  // ��Ŷ ������ ���� �ּ�

public:
	virtual bool DispatchRead(DWORD dwTrans, OVERLAPPED2* ov);
	virtual bool DispatchWrite(DWORD dwTrans, OVERLAPPED2* ov);

public:
	virtual bool WaitRecieve();
	HNetUser();
	virtual ~HNetUser();

};