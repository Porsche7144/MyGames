#pragma once
#include "Hcore.h"
#include "HSoundManager.h"

class Sample : public Hcore
{
	HSound*  m_pBGM;
	HSound*	 m_pEffect;

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

