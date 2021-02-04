#include "HNetUser.h"
#include "HServer.h"

//	 |p1------p1e:p2-------p2e:p3-------t----p3e-------|
//	 1, 패킷 헤더 > dwTrans
//	 2, 패킷 헤더 == dwTrans
//	 3, 패킷 헤더 < dwTrans
		// 3.1, 패킷 크기 == dwTrans
		// 3.2, 패킷 크기 > dwTrans
		// 3.3, 패킷 크기 < dwTrans

//	0    ~    10000
//	  50byte
//	 --------(s)9985,9990   10;
//	 임시버퍼 받는 잔여량을 저장
//	clear   0    ~    10000
//	 주 버퍼에 임시버퍼의 잔여량 데이터 복사
//	clear   0~5: 50byte   ~    10000

bool HNetUser::DispatchRead(DWORD dwTrans, OVERLAPPED2* ov)
{
	// 패킷이 g_iMaxDataBufferSize 보다 크거나 같으면
	if (m_iWritePos + dwTrans >= g_iMaxDataBufferSize)
	{
		// 패킷의 양이 g_iMaxDataBufferSize를 초과했으면 버퍼의 처음위치로 되돌린다
		// 이때 ReadPos에 패킷이 남아있으면 잔여패킷이 날아가므로
		// 잔여패킷을 WritePos에 담아준다.
		if (m_iReadPos > 0)
		{
			memmove(m_szDataBuf, &m_szDataBuf[m_iPacketPos], m_iReadPos);
		}
		m_iPacketPos = 0;
		m_iWritePos = m_iReadPos;
	}

	// 받은 데이터(패킷) 누적
	memcpy(&m_szDataBuf[m_iWritePos], m_szRecvBuf, dwTrans);
	m_iWritePos += dwTrans;
	m_iReadPos += dwTrans;

	if (m_iReadPos >= PACKET_HEADER_SIZE)
	{
		UPACKET* packet = (UPACKET*)m_szDataBuf[m_iPacketPos];

		// 1개 패킷 이상의 데이터가 존재하면
		if (packet->ph.len <= m_iReadPos)
		{
			do
			{
				HPacket hPacket;
				hPacket.pUser = this;
				memcpy(&hPacket, &m_szDataBuf[m_iPacketPos], packet->ph.len);

				// 패킷 풀에 완성 패킷을 넣어준다.
				I_Server.m_RecvPacketPool.AddPacket(hPacket);

				m_iReadPos -= packet->ph.len;
				m_iPacketPos += packet->ph.len;

				if (m_iReadPos < PACKET_HEADER_SIZE)
				{
					break;
				}
				UPACKET* packet = (UPACKET*)&m_szDataBuf[m_iPacketPos];
			}
			while (packet->ph.len <= m_iReadPos);
		}
	}

	WaitRecieve();

	return true;
}
bool HNetUser::DispatchWrite(DWORD dwTrans, OVERLAPPED2* ov)
{
	
	return true;
}

bool HNetUser::WaitRecieve()
{
	ZeroMemory(&m_ovRecv, sizeof(OVERLAPPED));
	m_ovRecv.iType = OVERLAPPED2::RECV_MODE;

	ZeroMemory(m_szRecvBuf, g_iRecvBufferSize);
	m_wsaRecvBuf.len = g_iRecvBufferSize;
	m_wsaRecvBuf.buf = m_szRecvBuf;
	DWORD cbTrans;
	DWORD dwFlags = 0;

	// WSARecv 연결 소켓으로부터 데이터를 읽어오는 함수
	// (소켓, WSABUF구조체를 가리키는 포인터, WSABUF구조체 갯수, 데이터 입력 완료시 데이터 바이트 크기를
	// 넘길 DWORD값, 함수의 호출 방식을 나타내는 플래그, WSAOVERLAPPED 구조체의 포인터, 
	// 수신작업 완료시 호출 될 완료 루틴의 포인터)
	int iRet = WSARecv(m_Sock, &m_wsaRecvBuf, 1, &cbTrans, &dwFlags, (OVERLAPPED*)&m_ovRecv, NULL);
	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}

HNetUser::HNetUser()
{
	m_bExit = false;
	m_iPacketPos = 0; // 패킷 시작주소
	m_iWritePos = 0;  // 패킷 데이터 저장 주소
	m_iReadPos = 0;	  // 패킷 데이터 저장 주소
}

HNetUser::~HNetUser()
{

}

