#include "Sample.h"


bool Sample::Init()
{
	return true;
}

bool Sample::Frame()
{
	if (g_Input.GetKey('P') == KEY_PUSH)
	{
		DWORD color = RGB(rand() / 255, rand() / 255, rand() / 255);
		HMsg msg = { {0,0,800,600}, L"aaaaa", color };
		
	}

	return true;
}

bool Sample::Render()
{
	return true;
}

bool Sample::Release()
{
	return true;
}

HGAME_RUN;