#include "Sample.h"

// �ʱ�ȭ �� �غ� �۾�
bool Sample::Init()
{
	g_SoundMgr.Init();
	m_pBGM = g_SoundMgr.Load("../../Data/Sound/sample1.mp3");
	m_pEffect = g_SoundMgr.Load("sample2.mp3");
	m_pBGM->Play();

	return true;
}

// ������ ������ ����۾�
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
		// �ð�����ȭ
		m_pBGM->Volume(0.33f);
	}
	if (g_Input.GetKey('5') == KEY_HOLD)
	{
		m_pBGM->Volume(-0.33f);
	}
	return true;
}

// ������ ������ ��� (��ο�, ������)
bool Sample::Render()
{
	g_SoundMgr.Render();
	return true;
}

// �Ҹ� �� ���� ��� ����
bool Sample::Release()
{
	g_SoundMgr.Release();
	return true;
}

GAMERUN;