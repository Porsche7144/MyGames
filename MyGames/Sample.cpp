#include <Windows.h>
#include <string>

//#define DECLARE_HANDLE(name) 
//struct name__
//{
//    int unused;
//};
//typedef struct name__ *name

// 윈도우 클래스를 운영체제에 등록
HINSTANCE hInst;
BOOL InitInstance(const WCHAR* szWindowClass, const WCHAR* szTitle, HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
	// 운영체제 ->메세지 상호통신(연락사무소)함수 = 윈도우프로시저(메세지큐) ->메세지 윈도우
	// 윈도우 -> 윈도우지원함수 -> 운영체제
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
	// 예매, 발급받기 전
	MyRegisterClass(hInstance, L"KGCA");

	if (!InitInstance(L"KGCA", L"xxxxx", hInstance, nCmdShow))
	{
		return FALSE;
	}

	MSG msg;

	// 기본 메시지 루프입니다:
	while (1)
	{
		// 메세지 큐에 있는 메세지 반환
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // MM_QUIT
		{
			TranslateMessage(&msg); // 키 번역
			DispatchMessage(&msg); // 프로시저 전달
			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			// 게임 로직이 돌아간다

			// 출력창에 출력하는 함수
			/*OutputDebugString(L"test");*/
			// Sleep(1000); // 1000 = 1초 대기

		/*	GameRun();*/
		}

	}

	//bool GameRun();
	//{
	//	// 게임 로직이 돌아간다

	//	// 출력창에 출력하는 함수
	//	OutputDebugString(L"test");
	//	// Sleep(1000); // 1000 = 1초 대기
	//}


	// 윈도우 생성

	MessageBox(NULL, L"test", L"sample", MB_OK);

}