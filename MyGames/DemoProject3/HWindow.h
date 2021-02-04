#pragma once
#include "Hstd.h"

// 윈도우 관련 기능만 추가.

class HWindow
{
public:
	HINSTANCE	 m_hInst;
	WNDCLASSEXW  m_wcex;
	MSG			 m_msg;
	WCHAR		 m_szClassName[256];

public:
	bool SetWindow(HINSTANCE hlnst);
	bool MsgProcess();


public:
	HWindow();
	virtual ~HWindow();

};







