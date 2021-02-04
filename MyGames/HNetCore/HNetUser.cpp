#include "HNetUser.h"
#include "HServer.h"

//	 |p1------p1e:p2-------p2e:p3-------t----p3e-------|
//	 1, ��Ŷ ��� > dwTrans
//	 2, ��Ŷ ��� == dwTrans
//	 3, ��Ŷ ��� < dwTrans
		// 3.1, ��Ŷ ũ�� == dwTrans
		// 3.2, ��Ŷ ũ�� > dwTrans
		// 3.3, ��Ŷ ũ�� < dwTrans

//	0    ~    10000
//	  50byte
//	 --------(s)9985,9990   10;
//	 �ӽù��� �޴� �ܿ����� ����
//	clear   0    ~    10000
//	 �� ���ۿ� �ӽù����� �ܿ��� ������ ����
//	clear   0~5: 50byte   ~    10000

bool HNetUser::DispatchRead(DWORD dwTrans, OVERLAPPED2* ov)
{
	// ��Ŷ�� g_iMaxDataBufferSize ���� ũ�ų� ������
	if (m_iWritePos + dwTrans >= g_iMaxDataBufferSize)
	{
		// ��Ŷ�� ���� g_iMaxDataBufferSize�� �ʰ������� ������ ó����ġ�� �ǵ�����
		// �̶� ReadPos�� ��Ŷ�� ���������� �ܿ���Ŷ�� ���ư��Ƿ�
		// �ܿ���Ŷ�� WritePos�� ����ش�.
		if (m_iReadPos > 0)
		{
			memmove(m_szDataBuf, &m_szDataBuf[m_iPacketPos], m_iReadPos);
		}
		m_iPacketPos = 0;
		m_iWritePos = m_iReadPos;
	}

	// ���� ������(��Ŷ) ����
	memcpy(&m_szDataBuf[m_iWritePos], m_szRecvBuf, dwTrans);
	m_iWritePos += dwTrans;
	m_iReadPos += dwTrans;

	if (m_iReadPos >= PACKET_HEADER_SIZE)
	{
		UPACKET* packet = (UPACKET*)m_szDataBuf[m_iPacketPos];

		// 1�� ��Ŷ �̻��� �����Ͱ� �����ϸ�
		if (packet->ph.len <= m_iReadPos)
		{
			do
			{
				HPacket hPacket;
				hPacket.pUser = this;
				memcpy(&hPacket, &m_szDataBuf[m_iPacketPos], packet->ph.len);

				// ��Ŷ Ǯ�� �ϼ� ��Ŷ�� �־��ش�.
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

	// WSARecv ���� �������κ��� �����͸� �о���� �Լ�
	// (����, WSABUF����ü�� ����Ű�� ������, WSABUF����ü ����, ������ �Է� �Ϸ�� ������ ����Ʈ ũ�⸦
	// �ѱ� DWORD��, �Լ��� ȣ�� ����� ��Ÿ���� �÷���, WSAOVERLAPPED ����ü�� ������, 
	// �����۾� �Ϸ�� ȣ�� �� �Ϸ� ��ƾ�� ������)
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
	m_iPacketPos = 0; // ��Ŷ �����ּ�
	m_iWritePos = 0;  // ��Ŷ ������ ���� �ּ�
	m_iReadPos = 0;	  // ��Ŷ ������ ���� �ּ�
}

HNetUser::~HNetUser()
{

}

