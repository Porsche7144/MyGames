// MyGames.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "MyGames.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

class _tagArea
{
public:
	bool bStart;
	POINT ptStart;
	POINT ptEnd;
};

_tagArea g_tArea;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYGAMES, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYGAMES));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYGAMES));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL; //MAKEINTRESOURCEW(IDC_MYGAMES);   메뉴
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

		// 마우스가 움직일 때 들어오는 메세지.
	case WM_MOUSEMOVE:
	{
		if (g_tArea.bStart)
		{
			g_tArea.ptEnd.x = lParam & 0x0000ffff;
			g_tArea.ptEnd.y = lParam >> 16;
			InvalidateRect(hWnd, NULL, TRUE);
		}
	}
		break;

	case WM_LBUTTONUP:
	{
		if (g_tArea.bStart)
		{
			g_tArea.bStart = false;
			g_tArea.ptEnd.x = lParam & 0x0000ffff;
			g_tArea.ptEnd.y = lParam >> 16;
			InvalidateRect(hWnd, NULL, TRUE);
		}
	}
		break;

		// 마우스 왼쪽 버튼을 눌렀을때 들어온다.
	case WM_LBUTTONDOWN: // 마우스 위치는 lParam에 들어온다.
	{
		if (!g_tArea.bStart)
		{
			g_tArea.bStart = true;
			g_tArea.ptStart.x = lParam & 0x0000ffff;
			g_tArea.ptStart.y = lParam >> 16;
			g_tArea.ptEnd = g_tArea.ptStart;

			// 강제로 WM_PAINT 메세지를 호출하는 함수.
			// 1번인자는 윈도우 핸들
			// 2번인자는 초기화 할 영역. NULL을 입력할 경우 전체화면을 대상으로 갱신
			// 3번인자는 TRUE일경우 현재화면을 지우고 갱신, False일 경우 현재화면을 지우지 않고 갱신
			InvalidateRect(hWnd, NULL, TRUE);
		}
	}
		break;

		// 키가 눌러졌을때 들어오는 메세지
		case WM_KEYDOWN:
			// 이 메세지가 들어올 경우 wParam에 어떤 키를 눌렀는지가 들어옴.
			switch (wParam)
			{
			case VK_ESCAPE:                      // ESC
				DestroyWindow(hWnd);
			}
			break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

			TextOut(hdc, 500, 250, TEXT("win32"), 5);
			TextOut(hdc, 250, 500, TEXT("win32"), 5); // 문자출력함수 (hdc, x좌표, y좌표, TEXT함수(" "), 글자수);
			TextOut(hdc, 100, 600, TEXT("win32"), 5);

			TCHAR strMouse[64] = {};

			// wsprintf() 유니코드 문자열을 만들어주는 함수.
			wsprintf(strMouse, TEXT("Start = x : %d, y: %d"), g_tArea.ptStart.x, g_tArea.ptStart.y);
			TextOut(hdc, 600, 30, strMouse, lstrlen(strMouse)); // lstrlen() 유니코드 문자의 길이를 구하는 함수.

			Rectangle(hdc, 100, 100, 300, 300); // 사각형 그리기 함수; (hdc, Left, Top, Right, Bottom);
			Ellipse(hdc, 100, 100, 300, 300); // 원그리기 함수
			
			MoveToEx(hdc, 300, 100, NULL); // 선그리기 시작점
			LineTo(hdc, 400, 150);  // 선그리기 종료지점
			LineTo(hdc, 500, 100);  // 연결해서 그리기 가능. 이전에 그려졌던 선이 시작점이 됨.

			MoveToEx(hdc, 500, 500, NULL);
			LineTo(hdc, 300, 400);

			if (g_tArea.bStart)
			{
				Rectangle(hdc, g_tArea.ptStart.x, g_tArea.ptStart.y, g_tArea.ptEnd.x, g_tArea.ptEnd.y);
				/*MoveToEx(hdc, g_tArea.ptStart.x, g_tArea.ptStart.y, NULL);
				LineTo(hdc, g_tArea.ptEnd.x, g_tArea.ptEnd.y);*/
			}
			

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
