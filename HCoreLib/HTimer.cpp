#include "HTimer.h"

float	g_fGametimer = 0.0f;
float	g_fSecondPerFrame = 0.0f;

// �ʱ�ȭ �� �غ� �۾�
bool HTimer::Init()
{
	m_fGametimer = 0.0f;
	m_fSecondPerFrame = 0.0f;
	m_iFPS = 0;

	// timeGetTime()  1/1000 ������ ��ȯ
	m_fBeforeTime = timeGetTime(); // �ʱ�ȭ�� �ð���ȯ

	return true;
}
// ������ ������ ����۾�
bool  HTimer::Frame()
{
	// ������ ���Խ� �ð���ȯ
	float fCurrentTime = timeGetTime();

	// ������ ���Խð� / �ʱ�ȭ�ð� = ������ ����ð�
	m_fSecondPerFrame = (fCurrentTime - m_fBeforeTime) / 1000.0f;
	
	// ������ ����ð� ����
	m_fGametimer += m_fSecondPerFrame;
	m_fBeforeTime = fCurrentTime;

	g_fGametimer = m_fGametimer;
	g_fSecondPerFrame = m_fSecondPerFrame;

	return true;

}
// ������ ������ ��ο�(������)
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
		// �ѹ� �������� 1������ ����.
		m_iFPS = 0;
	}
	m_iFPS++;
	return true;
}
// �Ҹ� �� ���� ��� ����
bool  HTimer::Release()
{
	return true;
}