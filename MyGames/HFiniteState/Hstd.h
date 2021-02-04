#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <tchar.h>
#include <map>
#include <list>
#include <algorithm>
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "HCoreLib.lib")
#pragma comment(lib, "fmod.lib")
#pragma comment(lib, "Msimg32.lib")

using namespace std;


// 구조체는 클래스 위로
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
	DWORD bHangul;
	DWORD bCapsLock;
	DWORD bNumLock;
};

struct HPoint
{
	float x;
	float y;
	HPoint() = default;
	HPoint(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	HPoint operator * (float fValue)
	{
		HPoint ret;
		ret.x = x * fValue;
		ret.y = y * fValue;
		return ret;
	}
	HPoint operator + (HPoint& p)
	{
		HPoint ret;
		ret.x = x + p.x;
		ret.y = y + p.y;
		return ret;
	}
	HPoint operator + (HPoint p)
	{
		HPoint ret;
		ret.x = x + p.x;
		ret.y = y + p.y;
		return ret;
	}
	HPoint operator - (HPoint& p)
	{
		HPoint ret;
		ret.x = x - p.x;
		ret.y = y - p.y;
		return ret;
	}
	float Length()
	{
		return sqrt(x*x + y * y);
	}
};

// 1. 게임 실행 시간 누적 기능
extern float		g_fGametimer;
// 2. 1프레임 경과 시간 저장
extern float		g_fSecondPerFrame;

extern HWND			g_hwnd;
extern HInputKey	g_ActionInput;
extern HINSTANCE	g_hInst;
extern POINT		g_MousePos;
extern RECT			g_rt;
extern HDC			g_hScreenDC;
extern HDC			g_hOffScreenDC;

template<class H> class HSingleton
{
public:
	static H& GetInstance()
	{
		static H SingleInstance;
		return SingleInstance;
	}
};