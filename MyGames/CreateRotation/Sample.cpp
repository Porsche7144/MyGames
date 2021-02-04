#include "Sample.h"
#include "BitmapManager.h"

void Sample::GetRotationBitmap(float fAngle, int w, int h)
{
	PatBlt(m_CreateDIB->m_hMemDC, 0, 0, w, h, PATCOPY);

	// SetGraphicsMode  지정된 디바이스 컨텍스트(DC)에 대한 그래픽 모드 설정.
	int iOldGraphic = SetGraphicsMode(m_CreateDIB->m_hMemDC, GM_ADVANCED);

	float fRadian = fAngle * (3.141592f / 180.0f);
	float fCos = cos(fRadian);
	float fSin = sin(fRadian);

	// XFORM  이차원 회전에 사용하는 구조체
	XFORM xForm;
	xForm.eM11 = fCos;
	xForm.eM12 = fSin;
	xForm.eM21 = -fSin;
	xForm.eM22 = fCos;

	// 회전된 이미지를 정 중앙으로 이동시킨다.
	// 이동된 위치가 원점이 된다.
	xForm.eDx = w / 2;
	xForm.eDy = h / 2;

	// SetWorldTransform 함수는 지정된 디바이스 컨텍스트에 대한 
	// 월드 공간과 페이지 공간 사이 이차원 선형 변환을 설정한다

		// 화면좌표계를 직각좌표계로
	SetWorldTransform(m_CreateDIB->m_hMemDC, &xForm);

	BitBlt(m_CreateDIB->m_hMemDC,
		-(m_pDlg->m_BitmapInfo.bmWidth/2), 
		-(m_pDlg->m_BitmapInfo.bmHeight/2),
		 m_pDlg->m_BitmapInfo.bmWidth, 
		 m_pDlg->m_BitmapInfo.bmHeight, 
		 m_pDlg->m_hMemDC, 0, 0, SRCCOPY);


	// 원래대로
	xForm.eM11 = 1;
	xForm.eM12 = 0;
	xForm.eM21 = 0;
	xForm.eM22 = 1;
	xForm.eDx = 0;
	xForm.eDy = 0;

		// 직각좌표계를 화면좌표계로
	SetWorldTransform(m_CreateDIB->m_hMemDC, &xForm);
	SetGraphicsMode(m_CreateDIB->m_hMemDC, iOldGraphic);
}

bool  Sample::Init()
{
	m_BGIntro.Init();
	m_BGIntro.Load(L"../../data/bitmap/KGCA.bmp");

	m_pDlg = g_BitMgr.Load(L"../../data/bitmap/Apopup_normal.bmp");


	// sqrt  매개변수 x로 들어온 숫자에 루트를 씌워서 계산한 값을 반환.
	// 즉, 루트 x를 구해주는 함수
	float fDistance = sqrt(m_pDlg->m_BitmapInfo.bmWidth * m_pDlg->m_BitmapInfo.bmWidth +
		m_pDlg->m_BitmapInfo.bmHeight * m_pDlg->m_BitmapInfo.bmHeight);

	m_CreateDIB = g_BitMgr.CreateLoad(
		fDistance+1,
		fDistance+1, L"test.bmp");


	return true;
}

bool  Sample::Frame()
{
	GetRotationBitmap(g_fGametimer*100.0f, m_CreateDIB->m_BitmapInfo.bmWidth,
						m_CreateDIB->m_BitmapInfo.bmWidth);

	return true;
}

bool  Sample::Render()
{
	m_BGIntro.Render();
	m_CreateDIB->DrawAlphaBlend(400 - 116, 300 - 116, g_fGametimer * 100);
	m_pDlg->DrawAlphaBlend(0, 0);
	return true;
}

bool Sample::Release()
{
	
	m_BGIntro.Release();

	return true;
}

GAMERUN;