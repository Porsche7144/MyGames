#include "HTimer.h"

float	g_fGametimer = 0.0f;
float	g_fSecondPerFrame = 0.0f;

// 초기화 및 준비 작업
bool HTimer::Init()
{
	m_fGametimer = 0.0f;
	m_fSecondPerFrame = 0.0f;
	m_iFPS = 0;

	// timeGetTime()  1/1000 단위로 반환
	m_fBeforeTime = timeGetTime(); // 초기화시 시간반환

	return true;
}
// 프레임 단위로 계산작업
bool  HTimer::Frame()
{
	// 프레임 진입시 시간반환
	float fCurrentTime = timeGetTime();

	// 프레임 진입시간 / 초기화시간 = 프레임 경과시간
	m_fSecondPerFrame = (fCurrentTime - m_fBeforeTime) / 1000.0f;
	
	// 프레임 경과시간 누적
	m_fGametimer += m_fSecondPerFrame;
	m_fBeforeTime = fCurrentTime;

	g_fGametimer = m_fGametimer;
	g_fSecondPerFrame = m_fSecondPerFrame;

	return true;

}
// 프레임 단위로 드로우(렌더링)
bool  HTimer::Render()
{
	static float fTimer = 0.0f;
	fTimer += m_fSecondPerFrame;
	if (fTimer >= 1.0f)
	{
		WCHAR szBuffer[256];
		ZeroMemory(szBuffer, sizeof(WCHAR) * 256);
		_stprintf_s(szBuffer, L"GameTimer = %10.1f, spf(%10.4f), fps(%d)", m_fGametimer, m_fSecondPerFrame, m_iFPS);
		TextOut(g_hOffScreenDC, 0, 0, szBuffer, wcslen(szBuffer - 1));
		OutputDebugString(szBuffer);

		// fTimer -= 1.0f;
		// 한번 돌때마다 1프레임 증가.
		m_iFPS = 0;
	}
	m_iFPS++;
	return true;
}
// 소멸 및 삭제 기능 구현
bool  HTimer::Release()
{
	return true;
}