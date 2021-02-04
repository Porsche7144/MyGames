#include "Sample.h"

// "../../Data/Bitmap/KGCABK.bmp"


// 초기화 및 준비 작업
bool Sample::Init()
{
	m_BitObPos.x = 400;
	m_BitObPos.y = 500;

	m_pBGbit = new HUserInterface;
	m_pHero = new HGameUser;
	m_pEnemy = new HNpcObject;
	
	m_pBGbit->Init();
	m_pHero->Init();
	m_pEnemy->Init();


	m_pBGbit->Load(L"../../Data/Bitmap/KGCABK.bmp");
	m_pHero->Load(L"../../Data/Bitmap/The_Kid.bmp", L"../../Data/Bitmap/The_Kidmask.bmp");
	m_pEnemy->Load(L"../../Data/Bitmap/bitmap1.bmp", L"../../Data/Bitmap/bitmap1mask.bmp");

	m_pBGbit->SetPosition({ 0, 0 });

	m_pHero->Set( { 0, 0, 220, 193 }, { (LONG)m_BitObPos.x, (LONG)m_BitObPos.y, 50, 30 }, 300.0f);
	m_pEnemy->Set({ 91, 1, 40, 60 }, { (LONG)m_BitObPos.x, 0, 40, 60 });

	m_Object.push_front(m_pBGbit);
	m_Object.insert(m_Object.end(), m_pHero);
	m_Object.insert(m_Object.end(), m_pEnemy);
	return true;
}
// 프레임 단위로 계산작업
bool Sample::Frame()
{
	for (BitmapObject* pBitObj : m_Object)
	{
		pBitObj->Frame();
	}
	return true;
}
// 프레임 단위로 출력 (드로우, 렌더링)
bool Sample::Render()
{
	for (BitmapObject* pBitObj : m_Object)
	{
		pBitObj->Render();
	}

	return true;
}
// 소멸 및 삭제 기능 구현
bool Sample::Release()
{

	for (BitmapObject* pBitObj : m_Object)
	{
		pBitObj->Release();
	}
	//m_pBGbit->Release();
	//m_pHero->Release();
	//m_pHeroMask->Release();
	//m_pEnemy->Release();
	//m_pEnemyMask->Release();
	return true;
}

GAMERUN;



// StretchBlt()
// SetStretchBitMode()
// TransparentBit()
// 