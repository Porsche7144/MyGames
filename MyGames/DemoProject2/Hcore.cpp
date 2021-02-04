#include "Hcore.h"


bool Hcore::GameInit()
{
	m_GameRun = true;
	Init();
	return true;
}
bool Hcore::GameRun()
{
	Frame();
	Render();

	//wstring msg;
	//msg = L"\nKGCA";
	////msg += std::to_wstring(iCount++);
	//OutputDebugString(msg.c_str());
	//Sleep(10);

	return true;
}

bool Hcore::Run()
{
	GameInit();
	while (m_GameRun)
	{
		if (MsgProcess() == false)
		{
			m_GameRun = false;
		}
		else
		{
			GameRun();
		}
	}
	GameRelease();
	return true;
}

bool Hcore::GameRelease()
{
	Release();
	return true;
}
