#include "Sample.h"
#include "BitmapManager.h"

bool  Sample::Init()
{
	m_BGIntro.Init();
	m_BGIntro.Load(L"../../data/bitmap/Loading800x600.bmp");

	m_pDlg = g_BitMgr.Load(L"../../data/bitmap/Apopup_normal.bmp");
	m_CreateDIB = g_BitMgr.CreateLoad(
		m_pDlg->m_BitmapInfo.bmWidth,
		m_pDlg->m_BitmapInfo.bmHeight, L"test.bmp");


	return true;
}

bool  Sample::Frame()
{
	for (int j = 0; j < m_CreateDIB->m_BitmapInfo.bmHeight; ++j)
	{
		LPBYTE pbSrcRGB = (LPBYTE) & ((DWORD*)m_CreateDIB->m_pRaster)[j * m_CreateDIB->m_BitmapInfo.bmWidth];
		LPBYTE pbDlgRGB = (LPBYTE) & ((DWORD*)m_pDlg->m_pRaster)[j * m_pDlg->m_BitmapInfo.bmWidth];
		for (int i = 0; i < m_CreateDIB->m_BitmapInfo.bmWidth; ++i)
		{
			if (i < 100)
			{
				pbSrcRGB[0] = (cosf(g_fGametimer) * 0.5f + 0.5f) * 255;
				pbSrcRGB[1] = (sinf(g_fGametimer) * 0.5f + 0.5f) * 255;
				pbSrcRGB[2] = 1.0f - (cosf(g_fGametimer) * 0.5f + 0.5f) * 255;// (cosf(g_fGameTimer)*0.5f + 0.5f) * 255;
				pbSrcRGB[3] = 255;// (cosf(g_fGameTimer)*0.5f + 0.5f) * 255;
			}
			else
			{
				pbSrcRGB[0] = pbDlgRGB[0];
				pbSrcRGB[1] = pbDlgRGB[1];
				pbSrcRGB[2] = pbDlgRGB[2];
				pbSrcRGB[3] = pbDlgRGB[3];
			}
			pbSrcRGB += 4;
			pbDlgRGB += 4;
		}
	}
	return true;
}

bool  Sample::Render()
{
	m_BGIntro.Render();
	m_CreateDIB->DrawAlphaBlend(400, 300);
	m_pDlg->DrawAlphaBlend(0, 0);
	return true;
}

bool Sample::Release()
{
	m_BGIntro.Release();
	return true;
}

GAMERUN;