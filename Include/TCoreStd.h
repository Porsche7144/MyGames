#pragma once
#include "TStd.h"
#define TGAME_STRAT int APIENTRY wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,	LPWSTR    lpCmdLine,int       nCmdShow){Sample demo;
#define TGAME_RUN	if (demo.SetWindow(hInstance)){demo.Run();}return 1;}
#define GAMERUN TGAME_STRAT;TGAME_RUN;