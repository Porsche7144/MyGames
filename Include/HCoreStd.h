#pragma once
#include "HStd.h"
#define HGAME_STRAT int APIENTRY wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,	LPWSTR    lpCmdLine,int       nCmdShow){Sample demo;
#define HGAME_LOOP	if (demo.SetWindow(hInstance)){demo.Run();}return 1;}
#define HGAME_RUN HGAME_STRAT;HGAME_LOOP;

#define HCORE_START int WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPWSTR    lpCmdLine,int       nCmdShow){Sample sample;
#define HCORE_WIN(s,x,y) if (sample.SetWindow(hInstance, L#s, x, y))	{	sample.Run();	} }
#define HCORE_RUN(s,x,y) HCORE_START;HCORE_WIN(s,x,y);
#define HCORE_END }