#pragma once
#include "Hcore.h"
#include "HBitmap.h"


struct HPoint
{
	float x = 0;
	float y = 0;
};

class Sample : public Hcore
{
public:

	HBitmap		m_BGbit;
	HBitmap		m_Hero;
	HBitmap		m_HeroMask;
	HBitmap		m_Enemy;
	HBitmap		m_EnemyMask;
	HPoint		m_ptPos;
	float		m_fSpeed;


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

