#include "HBitmap.h"


bool HBitmap::Load(wstring filename)
{
	m_hBitmap = (HBITMAP)LoadImage(
		g_hInst,
		filename.c_str(),
		IMAGE_BITMAP,
		0,
		0,
		LR_DEFAULTSIZE | LR_LOADFROMFILE);
	if (m_hBitmap == NULL)
	{
		return false;
	}

	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitmapInfo);

	m_hMemDC = CreateCompatibleDC(g_hScreenDC);
	SelectObject(m_hMemDC, m_hBitmap);

	COLORREF bkColor = RGB(255, 0, 0);
	HBRUSH m_hbrRed = CreateSolidBrush(bkColor);
	SelectObject(m_hMemDC, m_hbrRed);
	return true;
}

// 초기화 및 준비 작업
bool HBitmap::Init()
{
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


HBitmap::HBitmap()
{
	
}
HBitmap::~HBitmap()
{

}
