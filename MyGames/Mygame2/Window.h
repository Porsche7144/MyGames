#pragma once
#include <Windows.h>

class Window
{
public:
	// 멤버변수
	bool Window::SetWindow(HINSTANCE m_hInstance)
	{
		HINSTANCE m_hInstance;
		// 1. 윈도우 클래스 객채 등록
		WNDCLASSEX wc;
		wc.hinstance = m_hinstance;
		wc.ipszClassName = L"KGCAWINDOW";

		
		if (RegisterClassEx(&wc) == 0)
		{
			return false;
		}
		// 2. 윈도우 생성
		m_hWnd = CreateWindowEX();
		if (m_hWnd == NULL)
		{
			return false;
		}
		ShowWindow(m_hWnd, SW_SHOW);
		// 3. 윈도우 메세지 루프



		return false;
	}

	HINSTANCE m_hInstance;
	HWND m_hWnd;

public:
	bool SetWindow();

public:
	Window();
	virtual ~Window();

};

