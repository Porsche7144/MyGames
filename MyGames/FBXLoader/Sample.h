#pragma once
#include "HCore.h"
#include "HFbxObj.h"

class Sample : public HCore
{
public:
	HFbxObj		m_FbxObj;
	HModelObject m_ModelObj;

public:
	LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;

};

HGAME_RUN;