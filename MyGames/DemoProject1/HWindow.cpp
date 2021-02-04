#include "HWindow.h"


// 윈도우 프로시저
LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0); // WM_QUIT
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

bool HWindow::SetWindow(HINSTANCE hlnst)
{
	m_hInst = hlnst;
	ZeroMemory(&m_wcex, sizeof(m_wcex));
	m_wcex.cbSize = sizeof(WNDCLASSEX);
	m_wcex.style = CS_HREDRAW | CS_VREDRAW;
	m_wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	// 운영체제 ->메세지 상호통신(연락사무소)함수 = 윈도우프로시저(메세지큐) ->메세지 윈도우
	// 윈도우 -> 윈도우지원함수 -> 운영체제
	m_wcex.lpfnWndProc = WndProc;
	m_wcex.hInstance = hlnst;
	m_wcex.lpszClassName = L"KGCA";

	return RegisterClassExW(&m_wcex);



	// 윈도우 생성
	m_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		L"Windows",
		L"MyGame",
		WS_OVERLAPPEDWINDOW,
		//WS_OVERLAPPEDWINDOW (WS_OVERLAPPED     | \
	                             //WS_CAPTION        | \
	                             //WS_SYSMENU        | \
	                             //WS_THICKFRAME     | \
	                             //WS_MINIMIZEBOX    | \
	                             //WS_MAXIMIZEBOX)
		300, 300, 800, 600,
		nullptr, nullptr,
		m_hInst, nullptr);

	if (!m_hwnd)
	{
		return false;
	}
	ShowWindow(m_hwnd, SW_SHOW);
	//UpdateWindow(hWnd);

	return true;
}

// 메세지
bool HWindow::MsgProcess()
{
	if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);
		if (m_msg.message == WM_QUIT)
		{
			return false;
		}
	}
	return true;
}

HWindow::HWindow()
{
	ZeroMemory(&m_msg, sizeof(MSG));
}
HWindow::~HWindow()
{

}