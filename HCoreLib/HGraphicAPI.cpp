#include "HGraphicAPI.h"

HDC			g_hScreenDC;
HDC			g_hOffScreenDC;

// 초기화 및 준비 작업
bool HGraphicAPI::Init()
{
	// GetDC() 윈도우에 그리기 위한 함수.
	g_hScreenDC = GetDC(g_hwnd);
	//  CreateCompatibleDC()  DC와 호환하기 위한 함수. 더블버퍼링(가상화면)에 사용.
	g_hOffScreenDC = CreateCompatibleDC(g_hScreenDC);
	// CreateCompatibleBitmap()   DC에 생성된 값 기준으로 비트맵 생성.
	m_hOffScreenBit = CreateCompatibleBitmap(g_hScreenDC, g_rt.right, g_rt.bottom);
	// SelectObject()  DC에 저장된 GDI object의 핸들값을 바꿀때 사용.
	SelectObject(g_hOffScreenDC, m_hOffScreenBit);

	COLORREF bkColor = RGB(125, 125, 125);
	// CreateSolidBrush()  Brush 객체를 생성하고 그 객체의 핸들 값을 반환
	m_hbrColor = CreateSolidBrush(bkColor);
	SelectObject(g_hOffScreenDC, m_hbrColor);

	return true;
}
// 프레임 단위로 계산작업
bool HGraphicAPI::Frame()
{
	return true;
}
// 프레임 단위로 출력 (드로우, 렌더링)
bool HGraphicAPI::PreRender()
{
	// PatBlt()  해당 DC의 비트맵 영역을 패턴형태로 초기화
	PatBlt(g_hOffScreenDC, 0, 0, g_rt.right, g_rt.bottom, PATCOPY);

	return true;
}
bool HGraphicAPI::Render()
{
	return true;
}
bool HGraphicAPI::PostRender()
{
	// BitBlt()  m_hOffScreenDC에 저장한 가상화면을 화면에 출력하는 함수
	BitBlt(g_hScreenDC, 0, 0, g_rt.right, g_rt.bottom, g_hOffScreenDC, 0, 0, SRCCOPY);

	return true;
}
// 소멸 및 삭제 기능 구현
bool HGraphicAPI::Release()
{

	DeleteObject(m_hbrColor);
	DeleteObject(m_hOffScreenBit);
	ReleaseDC(g_hwnd, g_hOffScreenDC);
	ReleaseDC(g_hwnd, g_hScreenDC);

	return true;
}
