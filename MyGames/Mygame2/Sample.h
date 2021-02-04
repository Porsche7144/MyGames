#pragma once
#include "Window.h"
#include <windows.h>


class Sample
{
	Window m_Window;

	bool Window::SetWindow(HINSTANCE m_hInstance)
	{
		HINSTANCE m_hInstance;
		// 1. ������ Ŭ���� ��ä ���
		WNDCLASSEX wc;
		wc.hinstance = m_hinstance;
		wc.ipszClassName = L"KGCAWINDOW";


		if (RegisterClassEx(&wc) == 0)
		{
			return false;
		}
		// 2. ������ ����
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
		}


}