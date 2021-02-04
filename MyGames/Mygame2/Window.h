#pragma once
#include <Windows.h>

class Window
{
public:
	// �������
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
		// 3. ������ �޼��� ����



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

