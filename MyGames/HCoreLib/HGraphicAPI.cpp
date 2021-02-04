#include "HGraphicAPI.h"

HDC			g_hScreenDC;
HDC			g_hOffScreenDC;

// �ʱ�ȭ �� �غ� �۾�
bool HGraphicAPI::Init()
{
	// GetDC() �����쿡 �׸��� ���� �Լ�.
	g_hScreenDC = GetDC(g_hwnd);
	//  CreateCompatibleDC()  DC�� ȣȯ�ϱ� ���� �Լ�. ������۸�(����ȭ��)�� ���.
	g_hOffScreenDC = CreateCompatibleDC(g_hScreenDC);
	// CreateCompatibleBitmap()   DC�� ������ �� �������� ��Ʈ�� ����.
	m_hOffScreenBit = CreateCompatibleBitmap(g_hScreenDC, g_rt.right, g_rt.bottom);
	// SelectObject()  DC�� ����� GDI object�� �ڵ鰪�� �ٲܶ� ���.
	SelectObject(g_hOffScreenDC, m_hOffScreenBit);

	COLORREF bkColor = RGB(125, 125, 125);
	// CreateSolidBrush()  Brush ��ü�� �����ϰ� �� ��ü�� �ڵ� ���� ��ȯ
	m_hbrColor = CreateSolidBrush(bkColor);
	SelectObject(g_hOffScreenDC, m_hbrColor);

	return true;
}
// ������ ������ ����۾�
bool HGraphicAPI::Frame()
{
	return true;
}
// ������ ������ ��� (��ο�, ������)
bool HGraphicAPI::PreRender()
{
	// PatBlt()  �ش� DC�� ��Ʈ�� ������ �������·� �ʱ�ȭ
	PatBlt(g_hOffScreenDC, 0, 0, g_rt.right, g_rt.bottom, PATCOPY);

	return true;
}
bool HGraphicAPI::Render()
{
	return true;
}
bool HGraphicAPI::PostRender()
{
	// BitBlt()  m_hOffScreenDC�� ������ ����ȭ���� ȭ�鿡 ����ϴ� �Լ�
	BitBlt(g_hScreenDC, 0, 0, g_rt.right, g_rt.bottom, g_hOffScreenDC, 0, 0, SRCCOPY);

	return true;
}
// �Ҹ� �� ���� ��� ����
bool HGraphicAPI::Release()
{

	DeleteObject(m_hbrColor);
	DeleteObject(m_hOffScreenBit);
	ReleaseDC(g_hwnd, g_hOffScreenDC);
	ReleaseDC(g_hwnd, g_hScreenDC);

	return true;
}
