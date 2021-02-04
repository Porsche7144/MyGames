#pragma once
#include "Hcore.h"
#include "HUserInterface.h"
#include "HGameUser.h"
#include "HNpcObject.h"
#include "HSceneIntro.h"
#include "HSceneLobby.h"
#include "HScene.h"

enum ObjState
{
	BACKGROUND = 0,
	UI,
	USER,
	NPC
};


class Sample : public Hcore
{
public:
	HScene*					m_pCurrentScene;
	vector<HScene*>			m_TextLoad;
public:
	bool LoadScript(const TCHAR* filename);
	BitmapObject* ObjState(int iState);
	bool ImageCheck(FILE* fp, wstring filename);

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

