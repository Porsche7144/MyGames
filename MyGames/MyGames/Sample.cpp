#include <Windows.h>
#include <string>

//#define DECLARE_HANDLE(name) 
//struct name__
//{
//    int unused;
//};
//typedef struct name__ *name

// ������ Ŭ������ �ü���� ���
HINSTANCE hInst;
BOOL InitInstance(const WCHAR* szWindowClass, const WCHAR* szTitle, HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

	HWND hWnd = CreateWindowW(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		//WS_OVERLAPPEDWINDOW (WS_OVERLAPPED     | \
                             //WS_CAPTION        | \
                             //WS_SYSMENU        | \
                             //WS_THICKFRAME     | \
                             //WS_MINIMIZEBOX    | \
                             //WS_MAXIMIZEBOX)
		300, 300, 800, 600,
		nullptr, nullptr,
		hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, SW_SHOW);
	//UpdateWindow(hWnd);

	return TRUE;
}


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


ATOM MyRegisterClass(HINSTANCE hInstance, const WCHAR* szClassName)
{
	WNDCLASSEXW wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEXW));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	// �ü�� ->�޼��� ��ȣ���(�����繫��)�Լ� = ���������ν���(�޼���ť) ->�޼��� ������
	// ������ -> �����������Լ� -> �ü��
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = szClassName;

	return RegisterClassExW(&wcex);
}


int APIENTRY wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow)
{
	// ����, �߱޹ޱ� ��
	MyRegisterClass(hInstance, L"KGCA");

	if (!InitInstance(L"KGCA", L"xxxxx", hInstance, nCmdShow))
	{
		return FALSE;
	}

	MSG msg;

	// �⺻ �޽��� �����Դϴ�:
	while (1)
	{
		// �޼��� ť�� �ִ� �޼��� ��ȯ
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // MM_QUIT
		{
			TranslateMessage(&msg); // Ű ����
			DispatchMessage(&msg); // ���ν��� ����
			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			// ���� ������ ���ư���

			// ���â�� ����ϴ� �Լ�
			/*OutputDebugString(L"test");*/
			// Sleep(1000); // 1000 = 1�� ���

		/*	GameRun();*/
		}

	}

	//bool GameRun();
	//{
	//	// ���� ������ ���ư���

	//	// ���â�� ����ϴ� �Լ�
	//	OutputDebugString(L"test");
	//	// Sleep(1000); // 1000 = 1�� ���
	//}


	// ������ ����

	MessageBox(NULL, L"test", L"sample", MB_OK);

}