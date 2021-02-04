#include "Sample.h"
//GAMERUN;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	Sample demo;

	if (demo.SetWindow(hInstance))
	{
		demo.Run();
	}
	return 1;
}