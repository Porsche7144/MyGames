#include "Sample.h"

// "../../Data/Bitmap/KGCABK.bmp"


// 초기화 및 준비 작업
bool Sample::Init()
{
	m_BGbit.Load(L"../../Data/Bitmap/KGCABK.bmp");
	m_Hero.Load(L"../../Data/Bitmap/The_Kid.bmp");
	m_HeroMask.Load(L"../../Data/Bitmap/The_Kidmask.bmp");
	m_Enemy.Load(L"../../Data/Bitmap/bitmap1.bmp");
	m_EnemyMask.Load(L"../../Data/Bitmap/bitmap1mask.bmp");
	m_ptPos.x = 400;
	m_ptPos.y = 500;
	m_fSpeed = 200.0f;
	

	return true;
}
// 프레임 단위로 계산작업
bool Sample::Frame()
{

	if (g_ActionInput.bWKey)
	{
		m_ptPos.y -= m_fSpeed * g_fSecondPerFrame;
	}
	if (g_ActionInput.bAKey)
	{
		m_ptPos.x -= m_fSpeed * g_fSecondPerFrame;
	}
	if (g_ActionInput.bDKey)
	{
		m_ptPos.x += m_fSpeed * g_fSecondPerFrame;
	}
	if (g_ActionInput.bSKey)
	{
		m_ptPos.y += m_fSpeed * g_fSecondPerFrame;
	}
	
	return true;
}
// 프레임 단위로 출력 (드로우, 렌더링)
bool Sample::Render()
{

	RECT rtBGbit = { 0, 0, 800, 600 };
	m_BGbit.Render();


	RECT rtSrcBt1 = { 0, 0, 220, 193 };
	RECT rtDestBt1 = { m_ptPos.x, m_ptPos.y, 100, 60 };
	m_HeroMask.Draw(rtDestBt1, rtSrcBt1, SRCAND, 1);

	RECT rtSrcBt = { 0, 0, 220, 193 };
	RECT rtDestBt = { m_ptPos.x, m_ptPos.y, 100, 60 };
	// case  1 : 좌우반전, 2 : 상하반전, 3: 좌우상하반전, default : 0
	m_Hero.Draw(rtDestBt, rtSrcBt, SRCINVERT);
	m_HeroMask.Draw(rtDestBt1, rtSrcBt1, SRCINVERT);


	RECT rtSrcBtEnemy = { 90, 1, 40, 60 };
	RECT rtDestBtEnemy = { m_ptPos.x+200, m_ptPos.y, 100, 60 };

	RECT rtSrcBtEnemyMask = { 90, 1, 40, 60 };
	RECT rtDestBtEnemyMask = { m_ptPos.x+200, m_ptPos.y, 100, 60 };


	m_EnemyMask.Draw(rtDestBtEnemyMask, rtSrcBtEnemyMask, SRCAND);
	m_Enemy.Draw(rtDestBtEnemy, rtSrcBtEnemy, SRCINVERT);
	m_EnemyMask.Draw(rtDestBtEnemyMask, rtSrcBtEnemyMask, SRCINVERT);

	return true;
}
// 소멸 및 삭제 기능 구현
bool Sample::Release()
{
	m_BGbit.Release();
	m_Hero.Release();
	m_HeroMask.Release();
	return true;
}

GAMERUN;



// StretchBlt()
// SetStretchBitMode()
// TransparentBit()
// 