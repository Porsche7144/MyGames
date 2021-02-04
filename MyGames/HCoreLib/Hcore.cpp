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
	HInput::GetInstance().Init();
	g_Timer.Init();
	m_GraphicAPI.Init();
	HWrite::GetInstance().Init();
	g_SoundMgr.Init();
	// m_GraphicAPI.init();
	Init();
	return true;
}

bool Hcore::GameFrame()
{
	HInput::GetInstance().Frame();
	g_Timer.Frame();
	m_GraphicAPI.Frame();
	g_SoundMgr.Frame();
	HWrite::GetInstance().Frame();
	// m_GraphicAPI.Frame();
	Frame();

	return true;
}
bool Hcore::PreRender()
{
	m_GraphicAPI.PreRender();
	return true;
}
bool Hcore::PostRender()
{
	HInput::GetInstance().Render();
	g_Timer.Render();
	HWrite::GetInstance().Render();
	g_SoundMgr.Render();
	m_GraphicAPI.PostRender();
	return true;
}
bool Hcore::GameRender()
{
	PreRender();
	Render();
	PostRender();

	return true;
}
bool Hcore::GameRun()
{
	GameFrame();
	//m_Timer.Frame();

	GameRender();
	//m_Timer.Render();

	//wstring msg;
	//msg = L"\nKGCA";
	////msg += std::to_wstring(iCount++);
	//OutputDebugString(msg.c_str());

	return true;
}

void Hcore::MsgEvent(MSG msg)
{
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
			MsgEvent(m_msg);
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
	HInput::GetInstance().Release();
	g_Timer.Release();
	HWrite::GetInstance().Release();
	g_SoundMgr.Release();
	m_GraphicAPI.Release();
	return true;
}
