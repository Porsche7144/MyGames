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
		// 비동기의 작업 결과를 iocp큐에서 확인
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

		// 스레드 활성
		if (bReturn == TRUE)
		{
			// 정상 종료
			if (dwTransfor == 0)
			{
				pOV->iType = OVERLAPPED2::EXIT_MODE;
				PostQueuedCompletionStatus(iocp->m_hIOCP, 0, keyValue, (LPOVERLAPPED)pOV);
			}

			if (pOV->iType == OVERLAPPED2::RECV_MODE)
			{
				// 로드가 완료되면
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
			// 에러체크
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
	// CreateIoCompletionPort IOCP 생성
	// (적용한 소켓이나 파일의 핸들 IOCP 생성을 위해서는 INVALID_HANDLE_VALUE,
	// 존재하는 IOCP가 있을경우 그것의 핸들, 생성시에는 NULL값, send나 recv작업이 완료시
	// 그 작업이 일어난 핸들을 구별하기 위해 사용 생성시에는 NULL,
	// IOCP에서 동시에 동작시킬 쓰레드 개수. 0이면 현재 프로세스 수가 자동적으로 설정됨.)
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	
	for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
	{
		// CreateThread 쓰레드 생성
		// (핸들 보안속성, DWORD 스택크기(디폴트값 1mb), 쓰레드 작동함수, 함수호출시 전달할 인자,
		// 실행 후 대기 or 실행 0일경우 바로 실행, 쓰레드 생성시 쓰레드 ID리턴)
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
