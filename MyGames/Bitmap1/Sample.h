#pragma once
#include "Hcore.h"
#include "HUserInterface.h"
#include "HGameUser.h"
#include "HNpcObject.h"


class Sample : public Hcore
{
public:
	list<BitmapObject*>		m_Object;
	HDC						m_hMemDC;
	BitmapObject*			m_pBGbit;
	BitmapObject*			m_pHero;
	BitmapObject*			m_pHeroMask;
	BitmapObject*			m_pEnemy;
	BitmapObject*			m_pEnemyMask;
	HPoint					m_BitObPos;


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

