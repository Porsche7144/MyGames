#pragma once
#include <Windows.h>

class Window
{
public:
	// �������
	bool SetWindow(HINSTANCE m_hInstance);
	HINSTANCE m_hInstance;
	HWND m_hWnd;

public:

public:
	Window();
	virtual ~Window();

};

