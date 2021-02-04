#pragma once
#include "HNetStd.h"
#include "HNetUser.h"

#define MAX_WORKER_THREAD 3

class HIocp : public HNetSingleton<HIocp>
{
private:
	friend class HNetSingleton<HIocp>;

public:
	HANDLE m_hIOCP;
	HANDLE m_hWorkThread[MAX_WORKER_THREAD];
	DWORD m_dwThreadID[MAX_WORKER_THREAD];

public:
	bool DispatchRead(DWORD dwTrans, OVERLAPPED2* ov);
	bool DispatchWrite(DWORD dwTrans, OVERLAPPED2* ov);

public:
	bool Init();
	bool Run();
	bool Release();
	void SetBind(SOCKET sock, ULONG_PTR key);

public:
	HIocp();
	virtual ~HIocp();
};
#define I_IOCP HNetSingleton<HIocp>::GetInstance()

