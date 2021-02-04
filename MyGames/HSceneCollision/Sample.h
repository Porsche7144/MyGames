#pragma once
#include "Hcore.h"
#include "HSceneIntro.h"
#include "HSceneLobby.h"
#include "HSceneInGame.h"


class Sample : public Hcore
{
public:
	HScene*					m_pCurrentScene;
	vector<HScene*>			m_TextLoad;
	int						m_iCurrentScene;
public:

public:
	// 초기화 및 준비 작업
	bool Init();
	// 프레임 단위로 계산작업
	bool Frame();
	// 프레임 단위로 출력 (드로우, 렌더링)
	bool Render();
	// 소멸 및 삭제 기능 구현
	bool Release();
};

