#include "HThread.h"

void HThread::CreateThread()
{
	if (m_bStarted == true) return;
	m_hThread = _beginthreadex(NULL, 0, HandleRunner, (LPVOID)this, 0, &m_iThreadID);
	m_bStarted = true;
}

unsigned int __stdcall HThread::HandleRunner(LPVOID arg)
{
	HThread* pThread = (HThread*)arg;
	pThread->Run();
	return 0;
}

HThread::HThread() : m_bStarted(false)
{
}

HThread::~HThread()
{
	CloseHandle((HANDLE)m_hThread);
}