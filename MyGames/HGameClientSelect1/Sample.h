#pragma once
#include "HNetworkClient.h"
#include "Hcore.h"
#include "HSceneIntro.h"
#include "HSceneLobby.h"
#include "HSceneInGame.h"
#include "HFiniteStateMachine.h"

class Sample : public Hcore
{
	std::vector<HScene*>	m_SceneList;
	int						m_iCurrentScene;
public:
	HNetworkClientEventSelect	m_Network;
	HANDLE						m_hExecuteMutex;
	bool						m_bUseNet;
	std::string					m_szName;
public:
	bool  Init();
	bool  Frame();
	bool  Render();
	bool  Release();
public:
	bool  NetInit();
	bool  NetFrame();
	bool  NetRender();
	bool  NetRelease();
	void  MsgEvent(MSG msg) override;

	void  PlayDataSend(bool bRun = true);
public:
	void  SendTest(const char*pData);
};

