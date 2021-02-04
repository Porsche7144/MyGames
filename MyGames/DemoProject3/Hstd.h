#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <tchar.h>

using namespace std;

//#define TGAME_STRAT int APIENTRY wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,	LPWSTR    lpCmdLine,int       nCmdShow){Sample demo;
//#define TGAME_RUN	if (demo.SetWindow(hInstance)){demo.Run();}return 1;}
//#define GAMERUN TGAME_STRAT;TGAME_RUN;

// ����ü�� Ŭ���� ����
struct HInputKey
{
	DWORD bWKey;
	DWORD bAKey;
	DWORD bDKey;
	DWORD bSKey;
	DWORD bSpaceKey;
	DWORD bRightClick;
	DWORD bLeftClick;
	DWORD bMiddleClick;
	DWORD bExit;
};

// 1. ���� ���� �ð� ���� ���
extern float		g_fGametimer;
// 2. 1������ ��� �ð� ����
extern float		g_fSecondPerFrame;
extern HWND			g_hwnd;
extern HInputKey	g_ActionInput;


template<class H> class HSingleton
{
public:
	static H& GetInstance()
	{
		static H SingleInstance;
		return SingleInstance;
	}
};