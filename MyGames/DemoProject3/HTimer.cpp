#include "HTimer.h"

float	g_fGametimer = 0.0f;
float	g_fSecondPerFrame = 0.0f;

// �ʱ�ȭ �� �غ� �۾�
bool HTimer::init()
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
		char szBuffer[256] = { 0, };
		sprintf_s(szBuffer, "\n GameTimer = %f, spf(%10.4f), fps(%d)", m_fGametimer, m_fSecondPerFrame, m_iFPS);

		OutputDebugStringA(szBuffer);

		// 
		fTimer -= 1.0f;

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