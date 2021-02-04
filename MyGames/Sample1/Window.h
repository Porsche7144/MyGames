#pragma once
#include <Windows.h>

class Window
{
public:
	// ¸â¹öº¯¼ö
	bool SetWindow(HINSTANCE m_hInstance);
	HINSTANCE m_hInstance;
	HWND m_hWnd;

public:

public:
	Window();
	virtual ~Window();

};

