#include "HTimer.h"

namespace HBASIS_CORE_LIB
{
	float	 g_fGameTimer = 0.0f;
	float    g_fSecondPerFrame = 0.0f;
}

HTimer::HTimer()
{

}
HTimer::~HTimer()
{
	std::cout << "deconstructor";
}

bool  HTimer::Init()
{
	m_fGameTimer = 0.0f;
	m_fSecondPerFrame = 0.0f;
	m_iFPS = 0;
	m_fBeforeTime = timeGetTime();
	return true;
}
bool  HTimer::Reset()
{
	m_fSecondPerFrame = 0.0f;
	m_iFPS = 0;
	m_fBeforeTime = timeGetTime();
	return true;
}
bool  HTimer::Frame()
{
	// 1/1000 단위로 반환된다.
	float fCurrentTime = timeGetTime();
	m_fSecondPerFrame =
		(fCurrentTime - m_fBeforeTime) / 1000.0f;
	m_fGameTimer += m_fSecondPerFrame;
	m_fBeforeTime = fCurrentTime;

	g_fSecondPerFrame = m_fSecondPerFrame;
	g_fGameTimer = m_fGameTimer;
	return true;
}
bool  HTimer::Render()
{
	static float fTimer = 0.0f;
	fTimer += m_fSecondPerFrame;
	if (fTimer >= 1.0f)
	{
		ZeroMemory(m_szBuffer, sizeof(WCHAR) * 256);
		_stprintf_s(m_szBuffer,
			L"Runtime=%0.2f, SPF(%0.2f), FPS(%d)\n",
			m_fGameTimer,
			m_fSecondPerFrame,
			m_iFPS);
		//OutputDebugString(m_szBuffer);
		fTimer -= 1.0f;
		m_iFPS = 0;
	}

	m_iFPS++;
	return true;
}
bool  HTimer::Release()
{
	return true;
}