#include "HWindow.h"

 HWND		g_hwnd;
 HINSTANCE	g_hInst;
 RECT		g_rt;

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
	g_hInst = hlnst;
	ZeroMemory(&m_wcex, sizeof(m_wcex));
	m_wcex.cbSize = sizeof(WNDCLASSEX);
	m_wcex.style = CS_HREDRAW | CS_VREDRAW;
	m_wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	// 운영체제 ->메세지 상호통신(연락사무소)함수 = 윈도우프로시저(메세지큐) ->메세지 윈도우
	// 윈도우 -> 윈도우지원함수 -> 운영체제
	m_wcex.lpfnWndProc = WndProc;
	m_wcex.hInstance = g_hInst;
	m_wcex.lpszClassName = m_szClassName;

	if (RegisterClassEx(&m_wcex) == 0)
	{
		return false;
	}



	// 윈도우 생성
	RECT rt = { 0, 0, 800, 600 };

	// AdjustWindowRect  원하는 클라이언트 크기를 이용해서 윈도우 크기를 계산해주는 함수.
	// 첫번째 인수는 작업영역의 크기가 들어있는 RECT구조체를 설정.
	// 두번째 인수는 윈도우 스타일을 설정.
	// 세번째 인수는 메뉴를 추가할 것인지 아닌지를 설정. 원치않으면 FALSE.
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE);
	g_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		m_szClassName,
		L"MyGame", // Title name
		WS_OVERLAPPEDWINDOW,
		//WS_OVERLAPPEDWINDOW (WS_OVERLAPPED     | \
	                             //WS_CAPTION        | \
	                             //WS_SYSMENU        | \
	                             //WS_THICKFRAME     | \
	                             //WS_MINIMIZEBOX    | \
	                             //WS_MAXIMIZEBOX)
		0, 0, rt.right-rt.left, rt.bottom-rt.top,
		nullptr, nullptr,
		g_hInst, nullptr);

	if (g_hwnd == NULL)
	{
		return false;
	}

	// GetWindowRect() : 윈도우의 현재 위치와 크기를 구해준다. 
	// (left, top, right, bottom)좌표를 넘겨주며, 전체화면을 기준으로 한 좌표값이 된다.

	// GetClientRect() : 윈도우의 작업 영역 크기를 계산해주는 함수다.
	// left, top은 항상 0이면서 나머지 두 좌표는 right, top 이다. (그리기 영역의 크기가 된다.)
	GetClientRect(g_hwnd, &g_rt);
	ShowWindow(g_hwnd, SW_SHOW);
	//UpdateWindow(g_hwnd);

	return true;
}

// 메세지
bool HWindow::MsgProcess()
{
	while (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
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
	wcscpy_s(m_szClassName, L"HWindow");
}
HWindow::~HWindow()
{

}