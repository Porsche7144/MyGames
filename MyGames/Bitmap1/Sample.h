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
	// 초기화 및 준비 작업
	bool Init();
	// 프레임 단위로 계산작업
	bool Frame();
	// 프레임 단위로 출력 (드로우, 렌더링)
	bool Render();
	// 소멸 및 삭제 기능 구현
	bool Release();
};

