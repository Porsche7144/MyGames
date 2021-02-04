#include "Sample.h"

// "../../Data/Bitmap/KGCABK.bmp"

// 초기화 및 준비 작업
bool Sample::Init()
{
	m_iCurrentScene = 0;
	m_TextLoad.push_back(new HSceneIntro);
	m_TextLoad.push_back(new HSceneLobby);
	m_TextLoad.push_back(new HSceneInGame);
	m_TextLoad[0]->Load(L"GameIntro.txt");
	m_TextLoad[1]->Load(L"Lobby.txt");
	m_TextLoad[2]->Load(L"Ingame.txt");
	m_pCurrentScene = m_TextLoad[0];

	return true;
}
// 프레임 단위로 계산작업
bool Sample::Frame()
{
	//m_pCurrentScene = m_TextLoad[0];
	//m_pCurrentScene->Frame();
	if (m_pCurrentScene->m_bSceneChange == true)
	{
		m_pCurrentScene = m_TextLoad[m_iCurrentScene++];
	}
	//m_pCurrentScene = m_TextLoad[1];
	m_pCurrentScene->Frame();
	return true;
}
// 프레임 단위로 출력 (드로우, 렌더링)
bool Sample::Render()
{
	m_pCurrentScene->Render();
	return true;
}
// 소멸 및 삭제 기능 구현
bool Sample::Release()
{
	for (HScene* scene : m_TextLoad)
	{
		scene->Release();
	}

	return true;
}

GAMERUN;



// StretchBlt()
// SetStretchBitMode()
// TransparentBit()
// 