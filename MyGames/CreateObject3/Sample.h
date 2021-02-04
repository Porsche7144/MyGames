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
	// �ʱ�ȭ �� �غ� �۾�
	bool Init();
	// ������ ������ ����۾�
	bool Frame();
	// ������ ������ ��� (��ο�, ������)
	bool Render();
	// �Ҹ� �� ���� ��� ����
	bool Release();
};

