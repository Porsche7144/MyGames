#include "Core.h"

class Sample : public Core
{
	int APIENTRY wWinMain(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPWSTR    lpCmdLine,
		int       nCmdShow)
	{

		Sample demo;
		if (!demo.m_WIndow.SetWindow())
		{
			demo.m_Window.Run();
			return 0;
		}

		return 1;
	}
};
