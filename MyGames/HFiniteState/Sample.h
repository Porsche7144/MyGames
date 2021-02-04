#pragma once
#include "Hcore.h"
#include "HSceneIntro.h"
#include "HSceneLobby.h"
#include "HSceneInGame.h"
#include "HFiniteStateMachine.h"


class Sample : public Hcore
{
public:
	HPoint		m_BitObPos;

	vector<HEffect*>		m_EffectList;
	HScene*					m_pCurrentScene;
	vector<HScene*>			m_TextLoad;
	int						m_iCurrentScene;

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

