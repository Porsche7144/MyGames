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
	// 초기화 및 준비 작업
	bool Init();
	// 프레임 단위로 계산작업
	bool Frame();
	// 프레임 단위로 출력 (드로우, 렌더링)
	bool Render();
	// 소멸 및 삭제 기능 구현
	bool Release();
};

