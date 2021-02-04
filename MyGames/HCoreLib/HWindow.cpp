#include "HWindow.h"

 HWND		g_hwnd;
 HINSTANCE	g_hInst;
 RECT		g_rt;

// ������ ���ν���
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
	// �ü�� ->�޼��� ��ȣ���(�����繫��)�Լ� = ���������ν���(�޼���ť) ->�޼��� ������
	// ������ -> �����������Լ� -> �ü��
	m_wcex.lpfnWndProc = WndProc;
	m_wcex.hInstance = g_hInst;
	m_wcex.lpszClassName = m_szClassName;

	if (RegisterClassEx(&m_wcex) == 0)
	{
		return false;
	}



	// ������ ����
	RECT rt = { 0, 0, 800, 600 };

	// AdjustWindowRect  ���ϴ� Ŭ���̾�Ʈ ũ�⸦ �̿��ؼ� ������ ũ�⸦ ������ִ� �Լ�.
	// ù��° �μ��� �۾������� ũ�Ⱑ ����ִ� RECT����ü�� ����.
	// �ι�° �μ��� ������ ��Ÿ���� ����.
	// ����° �μ��� �޴��� �߰��� ������ �ƴ����� ����. ��ġ������ FALSE.
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

	// GetWindowRect() : �������� ���� ��ġ�� ũ�⸦ �����ش�. 
	// (left, top, right, bottom)��ǥ�� �Ѱ��ָ�, ��üȭ���� �������� �� ��ǥ���� �ȴ�.

	// GetClientRect() : �������� �۾� ���� ũ�⸦ ������ִ� �Լ���.
	// left, top�� �׻� 0�̸鼭 ������ �� ��ǥ�� right, top �̴�. (�׸��� ������ ũ�Ⱑ �ȴ�.)
	GetClientRect(g_hwnd, &g_rt);
	ShowWindow(g_hwnd, SW_SHOW);
	//UpdateWindow(g_hwnd);

	return true;
}

// �޼���
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