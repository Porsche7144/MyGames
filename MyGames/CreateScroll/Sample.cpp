#include "Sample.h"
#include "BitmapManager.h"

bool  Sample::Init()
{
	m_BGIntro.Init();
	m_BGIntro.Load(L"../../Data/Bitmap/KGCABK-copy.bmp");

	return true;
}

bool  Sample::Frame()
{
	return true;
}

bool  Sample::Render()
{
	static float nX = 0.0f;

	// nX+= g_fSecondPerFrame * 2;

	nX += (g_fSecondPerFrame+1) * 0.5;

	if (nX < 0)
	{
		BitBlt(g_hOffScreenDC, 0, 0, m_BGIntro.m_BitmapInfo.bmWidth - nX, m_BGIntro.m_BitmapInfo.bmHeight, 
				m_BGIntro.m_hMemDC, m_BGIntro.m_BitmapInfo.bmWidth + nX, 0, SRCCOPY);

		BitBlt(g_hOffScreenDC, -nX, 0, m_BGIntro.m_BitmapInfo.bmWidth, m_BGIntro.m_BitmapInfo.bmHeight,
				m_BGIntro.m_hMemDC, 0, 0, SRCCOPY);

	}
	else
	{
		BitBlt(g_hOffScreenDC, 0, 0, m_BGIntro.m_BitmapInfo.bmWidth, m_BGIntro.m_BitmapInfo.bmHeight,
			m_BGIntro.m_hMemDC, nX, 0, SRCCOPY);
	}


	if (nX + m_BGIntro.m_BitmapInfo.bmWidth >= m_BGIntro.m_BitmapInfo.bmWidth)
	{
		nX -= m_BGIntro.m_BitmapInfo.bmWidth;
	}

	return true;
}

bool Sample::Release()
{
	
	m_BGIntro.Release();

	return true;
}

GAMERUN;