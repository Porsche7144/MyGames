#include "HIocp.h"
#include "HSessionMgr.h"

DWORD WINAPI WorkerThread(LPVOID param)
{
	HIocp* iocp = (HIocp*)param;
	BOOL bReturn;
	DWORD dwTransfor;
	ULONG_PTR keyValue;
	OVERLAPPED2* pOV;

	while (1)
	{
		//if (WaitForSingleObject(g_hKillEvent, 1) == WAIT_OBJECT_0)
		//{
		//	break;
		//}
		//if (WaitForSingleObject(g_hEventEOF, 1) == WAIT_OBJECT_0)
		//{
		//	if (g_LargeRead.QuadPart <= g_LargeWrite.QuadPart)
		//	{
		//		::SetEvent(g_hKillEvent);
		//		continue;
		//	}
		//}
		// �񵿱��� �۾� ����� iocpť���� Ȯ��
		bReturn = ::GetQueuedCompletionStatus(iocp->m_hIOCP,
			&dwTransfor,
			&keyValue,
			(LPOVERLAPPED*)&pOV, 1);


		HNetUser* pUser = (HNetUser*)keyValue;
		if (pOV != nullptr && pOV->iType == OVERLAPPED2::EXIT_MODE)
		{
			I_Sesstion.DelUser(pUser);
			continue;
		}

		// ������ Ȱ��
		if (bReturn == TRUE)
		{
			// ���� ����
			if (dwTransfor == 0)
			{
				pOV->iType = OVERLAPPED2::EXIT_MODE;
				PostQueuedCompletionStatus(iocp->m_hIOCP, 0, keyValue, (LPOVERLAPPED)pOV);
			}

			if (pOV->iType == OVERLAPPED2::RECV_MODE)
			{
				// �ε尡 �Ϸ�Ǹ�
				if (pUser->DispatchRead(dwTransfor, pOV) == false)
				{
					pOV->iType = OVERLAPPED2::EXIT_MODE;
					PostQueuedCompletionStatus(iocp->m_hIOCP, 0, keyValue, (LPOVERLAPPED)pOV);
				}
			}
			if (pOV->iType == OVERLAPPED2::SEND_MODE)
			{
				if (pUser->DispatchWrite(dwTransfor, pOV) == false)
				{
					pOV->iType = OVERLAPPED2::EXIT_MODE;
					PostQueuedCompletionStatus(iocp->m_hIOCP, 0, keyValue, (LPOVERLAPPED)pOV);
				}
			}
		}
		else
		{
			// ����üũ
			DWORD dwError = GetLastError();
			if (dwError == WAIT_TIMEOUT) { continue; }
			closesocket(pUser->m_Sock);
		}

	}
	return 0;

}

bool HIocp::DispatchRead(DWORD dwTrans, OVERLAPPED2* ov)
{
	/*delete ov;
	g_LargeRead.QuadPart += dwTrans;

	T_OV* pov = new T_OV;
	ZeroMemory(pov, sizeof(T_OV));
	pov->ov.Offset = g_LargeWrite.LowPart;
	pov->ov.OffsetHigh = g_LargeWrite.HighPart;

	DWORD dwWriteByte;
	BOOL bResult = WriteFile(g_hWriteFile, g_pDataBuffer,
		dwTrans, &dwWriteByte, &pov->ov);
	if (bResult == FALSE)
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			return false;
		}
	}*/
	return true;
}
bool HIocp::DispatchWrite(DWORD dwTrans, OVERLAPPED2* ov)
{
	//delete ov;
	//g_LargeWrite.QuadPart += dwTrans;

	//std::cout << GetCurrentThreadId() << ":" << g_LargeWrite.QuadPart << std::endl;

	//if (WaitForSingleObject(g_hEventEOF, 1) == WAIT_OBJECT_0)
	//{
	//	if (g_LargeRead.QuadPart <= g_LargeWrite.QuadPart)
	//	{
	//		::SetEvent(g_hKillEvent);
	//		return true;
	//	}
	//}

	//T_OV* pov = new T_OV;
	//ZeroMemory(pov, sizeof(T_OV));
	//DWORD dwReadByte;

	//pov->ov.Offset = g_LargeRead.LowPart;
	//pov->ov.OffsetHigh = g_LargeRead.HighPart;

	//BOOL bResult = ReadFile(g_hReadFile, g_pDataBuffer,
	//	g_dwMaxReadSize, &dwReadByte, &pov->ov);
	//if (bResult == FALSE)
	//{
	//	if (GetLastError() != ERROR_IO_PENDING)
	//	{
	//		return false;
	//	}
	//}
	return true;
}

bool HIocp::Init()
{
	// CreateIoCompletionPort IOCP ����
	// (������ �����̳� ������ �ڵ� IOCP ������ ���ؼ��� INVALID_HANDLE_VALUE,
	// �����ϴ� IOCP�� ������� �װ��� �ڵ�, �����ÿ��� NULL��, send�� recv�۾��� �Ϸ��
	// �� �۾��� �Ͼ �ڵ��� �����ϱ� ���� ��� �����ÿ��� NULL,
	// IOCP���� ���ÿ� ���۽�ų ������ ����. 0�̸� ���� ���μ��� ���� �ڵ������� ������.)
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	
	for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
	{
		// CreateThread ������ ����
		// (�ڵ� ���ȼӼ�, DWORD ����ũ��(����Ʈ�� 1mb), ������ �۵��Լ�, �Լ�ȣ��� ������ ����,
		// ���� �� ��� or ���� 0�ϰ�� �ٷ� ����, ������ ������ ������ ID����)
		m_hWorkThread[iThread] = CreateThread(0, 0, WorkerThread, this, 0, &m_dwThreadID[iThread]);
	}

	return true;
}

bool HIocp::Run()
{
	return true;
}

bool HIocp::Release()
{
	CloseHandle(m_hIOCP);
	for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
	{
		CloseHandle(m_hWorkThread[iThread]);
	}

	return true;
}

void HIocp::SetBind(SOCKET sock, ULONG_PTR key)
{
	m_hIOCP = CreateIoCompletionPort((HANDLE)sock, m_hIOCP, key, 0);
}

HIocp::HIocp()
{
}

HIocp::~HIocp()
{
}
