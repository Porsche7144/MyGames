#pragma once
#include "HCore.h"
#include "HFbxObj.h"

#define MAX_BONE_MATRICES 255

class Sample : public HCore
{
public:
	HTexture* m_pNormalMap = nullptr;
	std::shared_ptr<HFbxObj> m_pFbxObj;

public:
	LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;

};

HGAME_RUN;