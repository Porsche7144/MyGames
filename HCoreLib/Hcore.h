#pragma once
#include "HWindow.h"
#include "HTimer.h"
#include "HInput.h"
#include "HWrite.h"
#include "HGraphicAPI.h"
#include "HSoundManager.h"

class Hcore : public HWindow
{
public:
	bool m_GameRun;
private:
	HTimer m_Timer;
	HInput m_Input;
	HWrite m_Write;
	HGraphicAPI m_GraphicAPI;

public:
	float GetGameTimer();
	float GetSPF();

public:
	virtual bool Init()
	{
		return true;
	}
	virtual bool Frame()
	{
		return true;
	}
	virtual bool PreRender();
	virtual bool PostRender();
	virtual bool Render()
	{
		return true;
	}
	virtual bool Release()
	{
		return true;
	}

private:
	bool GameInit();
	bool GameRelease();
	bool GameFrame();
	bool GameRun();
	bool GameRender();
public:
	bool Run();
};

