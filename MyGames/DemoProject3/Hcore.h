#pragma once
#include "HWindow.h"
#include "HTimer.h"
#include "HInput.h"

class Hcore : public HWindow
{
public:
	bool m_GameRun;
private:
	HTimer m_Timer;
	HInput m_Input;

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
	bool GameRun();
public:
	bool Run();
};

