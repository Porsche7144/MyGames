#include "Sample.h"
//GAMERUN;


bool  Sample::Init()
{
	m_fNpcAppere = 0.0f;

	TCHAR str[] = _T("NAME");
	TCHAR str2[] = _T("NAME");
	wcscpy_s(str, str2);
	return true;
}

bool  Sample::Frame()
{
	if (m_Input.GetKey(VK_LEFT) == KEY_PUSH)
	{
		MessageBox(g_hwnd, L"VK_LEFT", L"KEY", MB_OK);
	}
	if (m_Input.GetKey(VK_RIGHT) == KEY_PUSH)
	{
		MessageBox(g_hwnd, L"VK_RIGHT", L"KEY", MB_OK);
	}
	if (m_Input.GetKey(VK_UP) == KEY_PUSH)
	{
		MessageBox(g_hwnd, L"VK_UP", L"KEY", MB_OK);
	}
	if (m_Input.GetKey(VK_DOWN) == KEY_PUSH)
	{
		MessageBox(g_hwnd, L"VK_DOWN", L"KEY", MB_OK);
	}
	if (m_Input.GetKey('A') == KEY_PUSH)
	{
		MessageBox(g_hwnd, L"A", L"KEY", MB_OK);
	}
	if (m_Input.GetKey('W') == KEY_PUSH)
	{
		MessageBox(g_hwnd, L"W", L"KEY", MB_OK);
	}
	if (m_Input.GetKey('0') == KEY_PUSH)
	{
		MessageBox(g_hwnd, L"0", L"KEY", MB_OK);
	}
	if (m_Input.GetKey(VK_SPACE) == KEY_PUSH)
	{
		MessageBox(g_hwnd, L"SPACE", L"KEY", MB_OK);
	}

	RECT rect;


	m_fNpcAppere += GetSPF();
	m_Input.Frame();
	return true;
}

bool  Sample::Render()
{
	m_Input.Render();


	return true;
}

bool  Sample::Release()
{
	return true;
}

Sample::Sample()
{

}

Sample::~Sample()
{

}


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	Sample demo;

	if (demo.SetWindow(hInstance))
	{
		demo.Run();
	}
	return 1;
}

