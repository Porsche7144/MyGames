#pragma once
#include "HStd.h"

class HStopwatch
{
public:
	HStopwatch() { QueryPerformanceFrequency(&m_liPerFreq); Start(); }
	void Start() { QueryPerformanceCounter(&m_liPerfStart); }

	__int64 Now()
	{    // Start �� ȣ��� �� ����� �и��ʸ� ����
		LARGE_INTEGER liPerfNow;
		QueryPerformanceCounter(&liPerfNow);
		__int64 qwElapsedTime = (((liPerfNow.QuadPart - m_liPerfStart.QuadPart)));// / m_liPerFreq.QuadPart);
		return qwElapsedTime;
	}
	void Output(const TCHAR* pData)
	{
#ifdef _DEBUG
		TCHAR  strMessage[MAX_PATH] = { 0, };
		__int64 qwElapsedTime = Now();
		float Second = static_cast<float>(qwElapsedTime) / static_cast<float>(m_liPerFreq.QuadPart);
		_stprintf_s(strMessage, _T("<=====%s[%10.4f]====>\n"), pData, Second);
		OutputDebugString(strMessage);
#endif
	}
private:
	LARGE_INTEGER m_liPerFreq;    // �ʴ� ���
	LARGE_INTEGER m_liPerfStart;   // ���� ���
};

// 1, ���� ���� �ð� ���� ���
// 2, 1 Frame ��� �ð� ����
// 3, 1�� ���� ������ ī����
// ********* ��� Ŭ������ �ݵ�� ������ ���� �Լ��� �����Ѵ�. *******

class HTimer : public HSingleton<HTimer>
{
public:
	// 1, ���� ���� �ð� ���� ���
	float	m_fGameTimer;
	// 2, 1 Frame ��� �ð� ����
	float   m_fSecondPerFrame;
	// 3, 1�� ���� ������ ī����
	int		m_iFPS;

	WCHAR	m_szBuffer[256] = { 0, };
	float   m_fBeforeTime;
public:
	bool  Reset();
	// �ʱ�ȭ �� �غ� �۾� ����
	bool  Init();
	// ������ ������ ����۾�
	bool  Frame();
	// ������ ������ ��ο�(������)
	bool  Render();
	// �Ҹ� �� ���� ��� ����
	bool  Release();
public:
	HTimer();
	virtual ~HTimer();
};
#define g_Timer HTimer::GetInstance()
