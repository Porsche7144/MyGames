#pragma once
#include "HCore.h"
#include "HFbxObj.h"

class Sample : public HCore
{
public:
	std::vector<std::shared_ptr<HFbxObj>> m_ObjList;
	HFbxObj		m_FbxObj;
	HModelObject m_ModelObj;
	HDxObject m_DxObj;

public:
	LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;

};

HGAME_RUN;