#include "Sample.h"

// "../../Data/Bitmap/KGCABK.bmp"


// �ʱ�ȭ �� �غ� �۾�
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
// ������ ������ ����۾�
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
// ������ ������ ��� (��ο�, ������)
bool Sample::Render()
{

	RECT rtBGbit = { 0, 0, 800, 600 };
	m_BGbit.Render();


	RECT rtSrcBt1 = { 0, 0, 220, 193 };
	RECT rtDestBt1 = { m_ptPos.x, m_ptPos.y, 100, 60 };
	m_HeroMask.Draw(rtDestBt1, rtSrcBt1, SRCAND, 1);

	RECT rtSrcBt = { 0, 0, 220, 193 };
	RECT rtDestBt = { m_ptPos.x, m_ptPos.y, 100, 60 };
	// case  1 : �¿����, 2 : ���Ϲ���, 3: �¿���Ϲ���, default : 0
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
// �Ҹ� �� ���� ��� ����
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