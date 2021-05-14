#pragma once
#include "HStd.h"

class HStopwatch
{
public:
	HStopwatch() { QueryPerformanceFrequency(&m_liPerFreq); Start(); }
	void Start() { QueryPerformanceCounter(&m_liPerfStart); }

	__int64 Now()
	{    // Start 가 호출된 후 경과한 밀리초를 리턴
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
	LARGE_INTEGER m_liPerFreq;    // 초당 계수
	LARGE_INTEGER m_liPerfStart;   // 시작 계수
};

// 1, 게임 실행 시간 누적 기능
// 2, 1 Frame 경과 시간 저장
// 3, 1초 동안 프레임 카운터
// ********* 모든 클래스는 반드시 다음과 같은 함수를 구현한다. *******

class HTimer : public HSingleton<HTimer>
{
public:
	// 1, 게임 실행 시간 누적 기능
	float	m_fGameTimer;
	// 2, 1 Frame 경과 시간 저장
	float   m_fSecondPerFrame;
	// 3, 1초 동안 프레임 카운터
	int		m_iFPS;

	WCHAR	m_szBuffer[256] = { 0, };
	float   m_fBeforeTime;
public:
	bool  Reset();
	// 초기화 및 준비 작업 구현
	bool  Init();
	// 프레임 단위로 계산작업
	bool  Frame();
	// 프레임 단위로 드로우(렌더링)
	bool  Render();
	// 소멸 및 삭제 기능 구현
	bool  Release();
public:
	HTimer();
	virtual ~HTimer();
};
#define g_Timer HTimer::GetInstance()
