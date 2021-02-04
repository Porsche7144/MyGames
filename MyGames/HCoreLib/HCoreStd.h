#pragma once
#include "Hstd.h"

#define GAME_START int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow){Sample demo;
#define GAME_RUNNING if (demo.SetWindow(hInstance)){demo.Run();}return 1;}

#define GAMERUN  GAME_START;GAME_RUNNING;