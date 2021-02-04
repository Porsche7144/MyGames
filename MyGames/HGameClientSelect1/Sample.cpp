#include "Sample.h"
#include "HObjectManager.h"

// ftp://112.216.123.139 id:kgcauser ps:kgca!@34

bool  Sample::Init()
{
	//g_Fsm.AddStateTrasition(STATE_STAND, EVENT_TIMEMOVE, STATE_MOVE);
	//g_Fsm.AddStateTrasition(STATE_STAND, EVENT_POINTMOVE, STATE_MOVE);
	//g_Fsm.AddStateTrasition(STATE_STAND, EVENT_FINDTARGET, STATE_ATTACK);
	//g_Fsm.AddStateTrasition(STATE_MOVE, EVENT_STOPMOVE, STATE_STAND);
	//g_Fsm.AddStateTrasition(STATE_MOVE, EVENT_FINDTARGET, STATE_ATTACK);
	//g_Fsm.AddStateTrasition(STATE_ATTACK, EVENT_LOSTTARGET, STATE_STAND);

	m_iCurrentScene = 0;
	m_SceneList.push_back(new HSceneIntro);
	m_SceneList.push_back(new HSceneLobby);
	m_SceneList.push_back(new HSceneInGame);
	m_SceneList[0]->Load(L"../../data/script/Loading.txt");
	m_SceneList[1]->Load(L"../../data/script/Lobby.txt");
	m_SceneList[2]->Load(L"../../data/script/InGame.txt");
	HScene::m_pGamePlayer = m_SceneList[2];
	HScene::m_pGamePlayer->CreateScene();

	return NetInit();
}
bool  Sample::Frame()
{
	if (NetFrame() == false) return false;
	HScene::m_pCurrentScene->Frame();
	if (HScene::m_pCurrentScene->m_bGameFinish)
	{
		m_bGameRun = false;
	}
	return true;
}
bool  Sample::Release()
{
	if (NetRelease() == false) return false;
	for (HScene* scene : m_SceneList)
	{
		scene->Release();
	}
	return true;
}
bool  Sample::Render()
{
	HScene::m_pCurrentScene->Render();
	if (HScene::m_pCurrentScene->m_bSceneChange == true)
	{
		HScene::m_pCurrentScene->Reset();
		HScene::m_pCurrentScene = m_SceneList[HScene::m_pCurrentScene->m_iNextScene];
		HScene::m_pCurrentScene->CreateScene();
	}
	return NetRender();
}
GAMERUN;