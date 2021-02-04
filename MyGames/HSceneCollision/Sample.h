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
	// �ʱ�ȭ �� �غ� �۾�
	bool Init();
	// ������ ������ ����۾�
	bool Frame();
	// ������ ������ ��� (��ο�, ������)
	bool Render();
	// �Ҹ� �� ���� ��� ����
	bool Release();
};

