#include <windows.h>
#include <iostream>

#define MAX_WORKER_THREAD 4
#define g_dwMaxReadSize (4096)
HANDLE      g_hKillEvent;
HANDLE      g_hEventEOF;
HANDLE		g_hReadFile;
HANDLE		g_hWriteFile;
HANDLE      g_hIOCP;
HANDLE		g_hWorkThread[MAX_WORKER_THREAD];

char*   g_pOffsetData = nullptr;
char    g_pDataBuffer[g_dwMaxReadSize];
LARGE_INTEGER g_LargeWrite;
LARGE_INTEGER g_LargeRead;
CRITICAL_SECTION RecvCS;
CRITICAL_SECTION WriteCS;

struct T_OV
{
	OVERLAPPED ov;
};

bool DispatchRead(DWORD dwTrans, T_OV* ov)
{
	delete ov;
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
	}
	return true;
}
bool DispatchWrite(DWORD dwTrans, T_OV* ov)
{
	delete ov;
	g_LargeWrite.QuadPart += dwTrans;

	std::cout << GetCurrentThreadId() << ":" << g_LargeWrite.QuadPart << std::endl;

	if (WaitForSingleObject(g_hEventEOF, 1) == WAIT_OBJECT_0)
	{
		if (g_LargeRead.QuadPart <= g_LargeWrite.QuadPart)
		{
			::SetEvent(g_hKillEvent);
			return true;
		}
	}

	T_OV* pov = new T_OV;
	ZeroMemory(pov, sizeof(T_OV));
	DWORD dwReadByte;

	pov->ov.Offset = g_LargeRead.LowPart;
	pov->ov.OffsetHigh = g_LargeRead.HighPart;

	BOOL bResult = ReadFile(g_hReadFile, g_pDataBuffer,
		g_dwMaxReadSize, &dwReadByte, &pov->ov);
	if (bResult == FALSE)
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}

DWORD WINAPI WorkerThread(LPVOID param)
{
	BOOL bReturn;
	DWORD dwTransfor;
	ULONG_PTR keyValue;
	OVERLAPPED* pOV;
	while (1)
	{
		if (WaitForSingleObject(g_hKillEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}
		if (WaitForSingleObject(g_hEventEOF, 1) == WAIT_OBJECT_0)
		{
			if (g_LargeRead.QuadPart <= g_LargeWrite.QuadPart)
			{
				::SetEvent(g_hKillEvent);
				continue;
			}
		}
		// 비동기의 작업 결과를 iocp큐에서 확인
		bReturn = ::GetQueuedCompletionStatus(g_hIOCP,
			&dwTransfor,
			&keyValue,
			&pOV, 1);

		// 스레드 활성
		if (bReturn == TRUE)
		{
			T_OV* tOV = (T_OV*)pOV;
			if (keyValue == 1000)
			{
				// 로드가 완료되면
				if (DispatchRead(dwTransfor, tOV) == false)
				{
					::SetEvent(g_hKillEvent);
				}
			}
			if (keyValue == 2000)
			{
				if (DispatchWrite(dwTransfor, tOV) == false)
				{
					::SetEvent(g_hKillEvent);
				}
			}
		}
		else
		{
			// 에러체크
			DWORD dwError = GetLastError();
			if (dwError == WAIT_TIMEOUT) { continue; }
			if (pOV != NULL)
			{
				if (dwError == ERROR_HANDLE_EOF)
				{
					::SetEvent(g_hEventEOF);
					std::wcout << dwError << "ERROR_HANDLE_EOF" << std::endl;
				}
			}
			else
			{
				std::wcout << dwError << std::endl;
			}
		}

	}
	return 0;
}
void main()
{
	g_LargeWrite.QuadPart = 0;
	g_LargeRead.QuadPart = 0;
	g_hKillEvent = ::CreateEvent(0, TRUE, FALSE, 0);
	g_hEventEOF = ::CreateEvent(0, TRUE, FALSE, 0);

	std::wstring readname = L"sql";
	std::wstring writename = L"Copysql";
	std::wstring ext = L".iso";
	readname += ext;
	writename += ext;
	g_hReadFile = CreateFile(readname.c_str(),
		GENERIC_READ | GENERIC_WRITE, 0,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);
	if (g_hReadFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	g_hWriteFile = CreateFile(writename.c_str(),
		GENERIC_READ | GENERIC_WRITE, 0,
		NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);

	// 비동기 작업의 결과를 IOCP큐에 넣어준다.
	g_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	::CreateIoCompletionPort(g_hReadFile, g_hIOCP, 1000, 0);
	::CreateIoCompletionPort(g_hWriteFile, g_hIOCP, 2000, 0);

	// 일반적으로 '스레드 풀' 이라고 부른다.
	// 스레드 4개
	DWORD dwID;
	for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
	{
		g_hWorkThread[iThread] =
			::CreateThread(0, 0, WorkerThread, nullptr, 0, &dwID);
	}

	// Start
	T_OV* pov = new T_OV;
	ZeroMemory(pov, sizeof(T_OV));
	DWORD dwReadByte;
	BOOL bResult = ReadFile(g_hReadFile, g_pDataBuffer,
		g_dwMaxReadSize, &dwReadByte, &pov->ov);

	// 대기했다가 종료
	WaitForMultipleObjects(MAX_WORKER_THREAD, g_hWorkThread, TRUE, INFINITE);

	CloseHandle(g_hKillEvent);
	CloseHandle(g_hEventEOF);
	CloseHandle(g_hReadFile);
	CloseHandle(g_hWriteFile);
}