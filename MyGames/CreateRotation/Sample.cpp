#include "Sample.h"
#include "BitmapManager.h"

void Sample::GetRotationBitmap(float fAngle, int w, int h)
{
	PatBlt(m_CreateDIB->m_hMemDC, 0, 0, w, h, PATCOPY);

	// SetGraphicsMode  ������ ����̽� ���ؽ�Ʈ(DC)�� ���� �׷��� ��� ����.
	int iOldGraphic = SetGraphicsMode(m_CreateDIB->m_hMemDC, GM_ADVANCED);

	float fRadian = fAngle * (3.141592f / 180.0f);
	float fCos = cos(fRadian);
	float fSin = sin(fRadian);

	// XFORM  ������ ȸ���� ����ϴ� ����ü
	XFORM xForm;
	xForm.eM11 = fCos;
	xForm.eM12 = fSin;
	xForm.eM21 = -fSin;
	xForm.eM22 = fCos;

	// ȸ���� �̹����� �� �߾����� �̵���Ų��.
	// �̵��� ��ġ�� ������ �ȴ�.
	xForm.eDx = w / 2;
	xForm.eDy = h / 2;

	// SetWorldTransform �Լ��� ������ ����̽� ���ؽ�Ʈ�� ���� 
	// ���� ������ ������ ���� ���� ������ ���� ��ȯ�� �����Ѵ�

		// ȭ����ǥ�踦 ������ǥ���
	SetWorldTransform(m_CreateDIB->m_hMemDC, &xForm);

	BitBlt(m_CreateDIB->m_hMemDC,
		-(m_pDlg->m_BitmapInfo.bmWidth/2), 
		-(m_pDlg->m_BitmapInfo.bmHeight/2),
		 m_pDlg->m_BitmapInfo.bmWidth, 
		 m_pDlg->m_BitmapInfo.bmHeight, 
		 m_pDlg->m_hMemDC, 0, 0, SRCCOPY);


	// �������
	xForm.eM11 = 1;
	xForm.eM12 = 0;
	xForm.eM21 = 0;
	xForm.eM22 = 1;
	xForm.eDx = 0;
	xForm.eDy = 0;

		// ������ǥ�踦 ȭ����ǥ���
	SetWorldTransform(m_CreateDIB->m_hMemDC, &xForm);
	SetGraphicsMode(m_CreateDIB->m_hMemDC, iOldGraphic);
}

bool  Sample::Init()
{
	m_BGIntro.Init();
	m_BGIntro.Load(L"../../data/bitmap/KGCA.bmp");

	m_pDlg = g_BitMgr.Load(L"../../data/bitmap/Apopup_normal.bmp");


	// sqrt  �Ű����� x�� ���� ���ڿ� ��Ʈ�� ������ ����� ���� ��ȯ.
	// ��, ��Ʈ x�� �����ִ� �Լ�
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