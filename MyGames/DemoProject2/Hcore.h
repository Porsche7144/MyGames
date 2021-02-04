#pragma once
#include "HWindow.h"

class Hcore : public HWindow
{
public:
	bool m_GameRun;

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

