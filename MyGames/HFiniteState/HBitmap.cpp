#include "HBitmap.h"


// 초기화 및 준비 작업
bool HBitmap::Init()
{
	m_BlendFunction.BlendOp = AC_SRC_OVER;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.SourceConstantAlpha = 255;
	m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;

	return true;
}

HBITMAP  HBitmap::LoadDIB(wstring filename)
{
	// api file I/O
	HANDLE hFile = CreateFile(filename.c_str(),
		GENERIC_READ, 0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	//if (hFile == INVALID_HANDLE_VALUE)
	//{
	//	return false;
	//}
	BITMAPFILEHEADER header;
	DWORD dwRead;
	BOOL bRet = ReadFile(hFile, &header, sizeof(BITMAPFILEHEADER),
		&dwRead, NULL);

	DWORD dwDataSize = header.bfOffBits -
		sizeof(BITMAPFILEHEADER);

	BITMAPINFO* pBitInfo =
		(BITMAPINFO*)malloc(dwDataSize);

	bRet = ReadFile(hFile, pBitInfo, dwDataSize, &dwRead, NULL);

	PVOID pRaster = 0;
	m_hBitmap = CreateDIBSection(g_hScreenDC,
		pBitInfo, DIB_RGB_COLORS,
		(LPVOID*)(&m_pRaster), NULL, 0);

	ReadFile(hFile, m_pRaster,
		header.bfSize - header.bfOffBits,
		&dwRead, NULL);

	free(pBitInfo);
	CloseHandle(hFile);
	return m_hBitmap;
}

bool  HBitmap::Load(wstring filename)
{
	/*m_hBitmap = (HBITMAP)LoadImage(g_hInstance,
		filename.c_str(),
		IMAGE_BITMAP,
		0,
		0,
		LR_DEFAULTSIZE | LR_LOADFROMFILE);*/
	m_hBitmap = LoadDIB(filename.c_str());
	if (m_hBitmap == NULL)
	{
		return false;
	}
	m_hMemDC = CreateCompatibleDC(g_hScreenDC);
	SelectObject(m_hMemDC, m_hBitmap);

	COLORREF bkColor = RGB(255, 0, 0);
	HBRUSH m_hbrRed = CreateSolidBrush(bkColor);
	SelectObject(m_hMemDC, m_hbrRed);

	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitmapInfo);
	if (m_BitmapInfo.bmBitsPixel == 32)
	{
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	}
	return true;
}
bool HBitmap::CreateDIB(LONG iWidth, LONG iHeight)
{
	BITMAPINFO bmi;
	memset(&bmi.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biWidth = iWidth;
	bmi.bmiHeader.biHeight = iHeight;
	bmi.bmiHeader.biPlanes = 1;

	m_hBitmap = CreateDIBSection(g_hScreenDC, (BITMAPINFO*)&bmi,
		DIB_RGB_COLORS, (LPVOID*)&m_pRaster, NULL, 0);


	for (int j = 0; j < iHeight; ++j)
	{
		LPBYTE pbSrcRGB = (LPBYTE) & ((DWORD*)m_pRaster)[j * iWidth];
		for (int i = 0; i < iWidth; ++i)
		{
			pbSrcRGB[0] = 0;
			pbSrcRGB[1] = 0;
			pbSrcRGB[2] = 0;
			pbSrcRGB[3] = 255;
			pbSrcRGB += 4;
		}
	}

	m_hMemDC = CreateCompatibleDC(g_hScreenDC);
	SelectObject(m_hMemDC, m_hBitmap);

	COLORREF bkColor = RGB(255, 0, 0);
	HBRUSH m_hbrRed = CreateSolidBrush(bkColor);
	SelectObject(m_hMemDC, m_hbrRed);

	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitmapInfo);
	if (m_BitmapInfo.bmBitsPixel == 32)
	{
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	}
	return true;

}

// 프레임 단위로 계산작업
bool HBitmap::Frame()
{

	return true;
}
// 프레임 단위로 출력 (드로우, 렌더링)
bool HBitmap::Render()
{

	BitBlt(g_hOffScreenDC, 0, 0, g_rt.right, g_rt.bottom, m_hMemDC, 0, 0, SRCCOPY);

	return true;
}

bool HBitmap::Draw(RECT rt, RECT src, DWORD op, int Select)
{

	// BitBlt(g_hOffScreenDC, rt.left, rt.top, rt.right, rt.bottom, m_hMemDC, src.left, src.top, SRCCOPY);

	// StretchBlt(g_hOffScreenDC, rt.left, rt.top, rt.right, rt.bottom, m_hMemDC, src.left, src.top, src.right, src.bottom, op);
	// SetStretchBltMode(g_hOffScreenDC, HALFTONE);
	switch (Select)
	{
	case 0:
		// 원본 그대로
		StretchBlt(g_hOffScreenDC, rt.left, rt.top, rt.right, rt.bottom, m_hMemDC, src.left, src.top, src.right, src.bottom, op);
		break;
	case 1:
		// 좌우반전
		StretchBlt(g_hOffScreenDC, rt.left+rt.right, rt.top, -rt.right, rt.bottom, m_hMemDC, src.left, src.top, src.right, src.bottom, op);
		break;
	case 2:
		// 상하반전
		StretchBlt(g_hOffScreenDC, rt.left, rt.top+rt.bottom, rt.right, -rt.bottom, m_hMemDC, src.left, src.top, src.right, src.bottom, op);
		break;
	case 3:
		// 상하좌우반전
		StretchBlt(g_hOffScreenDC, rt.left+ rt.right, rt.top + rt.bottom, -rt.right, -rt.bottom, m_hMemDC, src.left, src.top, src.right, src.bottom, op);
		break;
	default:
		// 원본 그대로
		StretchBlt(g_hOffScreenDC, rt.left, rt.top, rt.right, rt.bottom, m_hMemDC, src.left, src.top, src.right, src.bottom, op);
		break;
	}

	// 이미지를 확대하거나 축소할때 픽셀을 보정해주는 함수
	// SetStretchBltMode(g_hOffScreenDC, SRCCOPY);

	return true;
}

// 소멸 및 삭제 기능 구현
bool HBitmap::Release()
{

	DeleteObject(m_hBitmap);
	ReleaseDC(g_hwnd, m_hMemDC);
	return true;
}

bool HBitmap::DrawTransParent(RECT desk, RECT src, DWORD color)
{
	TransparentBlt(g_hOffScreenDC, desk.left, desk.top, desk.right, desk.bottom, m_hMemDC,
					src.left, src.top, src.right, src.bottom, color);
	return true;
}

bool  HBitmap::DrawAlphaBlend(LONG x, LONG y,
	BYTE alpha,
	DWORD op,
	int iType)
{
	RECT desk = { x,y, m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight };
	RECT src = { 0,0,m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight };
	m_BlendFunction.BlendOp = AC_SRC_OVER;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.SourceConstantAlpha = alpha;
	//(cosf(g_fGameTimer)*0.5f + 0.5f) * 255;
	if (m_BitmapInfo.bmBitsPixel == 32)
	{
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	}

	AlphaBlend(g_hOffScreenDC,
		desk.left, desk.top,
		desk.right, desk.bottom,
		m_hMemDC,
		src.left, src.top,
		src.right, src.bottom, m_BlendFunction);
	return true;
}
bool  HBitmap::DrawAlphaBlend(RECT desk,
	RECT src,
	BYTE alpha,
	DWORD op,
	int iType)
{
	m_BlendFunction.BlendOp = AC_SRC_OVER;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.SourceConstantAlpha = alpha;
	//(cosf(g_fGameTimer)*0.5f + 0.5f) * 255;
	if (m_BitmapInfo.bmBitsPixel == 32)
	{
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	}

	AlphaBlend(g_hOffScreenDC,
		desk.left, desk.top,
		desk.right, desk.bottom,
		m_hMemDC,
		src.left, src.top,
		src.right, src.bottom, m_BlendFunction);
	return true;
}

bool  HBitmap::DrawAlphaBlend(RECT desk,
	RECT src,
	BLENDFUNCTION bf,
	DWORD op,
	int iType)
{
	AlphaBlend(g_hOffScreenDC,
		desk.left, desk.top,
		desk.right, desk.bottom,
		m_hMemDC,
		src.left, src.top,
		src.right, src.bottom, bf);
	return true;
}

HBitmap::HBitmap()
{
	
}
HBitmap::~HBitmap()
{

}
