#include "Sample.h"

// 초기화 및 준비 작업
bool Sample::Init()
{
	g_SoundMgr.Init();
	m_pBGM = g_SoundMgr.Load("../../Data/Sound/sample1.mp3");
	m_pEffect = g_SoundMgr.Load("sample2.mp3");
	m_pBGM->Play();

	return true;
}

// 프레임 단위로 계산작업
bool Sample::Frame()
{
	g_SoundMgr.Frame();

	if (g_Input.GetKey('1') == KEY_PUSH)
	{
		//m_pBG->Stop();
		m_pEffect->PlayEffect();

	}
	if (g_Input.GetKey('2') == KEY_PUSH)
	{
		m_pBGM->Play();
	}
	if (g_Input.GetKey('3') == KEY_PUSH)
	{
		m_pBGM->Paused();
	}
	if (g_Input.GetKey('4') == KEY_HOLD)
	{
		// 시간동기화
		m_pBGM->Volume(0.33f);
	}
	if (g_Input.GetKey('5') == KEY_HOLD)
	{
		m_pBGM->Volume(-0.33f);
	}
	return true;
}

// 프레임 단위로 출력 (드로우, 렌더링)
bool Sample::Render()
{
	g_SoundMgr.Render();
	return true;
}

// 소멸 및 삭제 기능 구현
bool Sample::Release()
{
	g_SoundMgr.Release();
	return true;
}

GAMERUN;