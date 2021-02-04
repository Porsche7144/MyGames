#pragma once
#include "Window.h"
#include <windows.h>


class Sample
{
	Window m_Window;

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
};

bool WINDOW::Run()
{
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
		}


}