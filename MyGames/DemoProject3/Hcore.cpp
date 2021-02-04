#include "Hcore.h"

float Hcore::GetGameTimer()
{
	return m_Timer.m_fGametimer;
}

float Hcore::GetSPF()
{
	return m_Timer.m_fSecondPerFrame;
}

bool Hcore::GameInit()
{
	m_GameRun = true;
	m_Timer.init();
	Init();
	return true;
}
bool Hcore::GameRun()
{
	Frame();
	//m_Timer.Frame();

	Render();
	//m_Timer.Render();

	//wstring msg;
	//msg = L"\nKGCA";
	////msg += std::to_wstring(iCount++);
	//OutputDebugString(msg.c_str());

	return true;
}

bool Hcore::Run()
{
	GameInit();


	while (m_GameRun)
	{
		if(PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&m_msg);
			DispatchMessage(&m_msg);
			if (m_msg.message == WM_QUIT)
			{
				m_GameRun = false;
			}
		}

		//if (MsgProcess() == false)
		//{
		//	m_GameRun = false;
		//}
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
