#pragma once
#include <fbxsdk.h>
#include "HCamera.h"
#include "HFbxObj.h"

#define MAX_BONE_MATRICES 255

class HModel : public HFbxObj
{
public:
	std::shared_ptr<HFbxObj> m_pFbxObj;
	Matrix matWorld = Matrix::Identity;
	Matrix matParent = Matrix::Identity;
	HCamera* m_pMainCamera;
	bool bDash = true;

public:
	std::string m_ObjName;

public:
	// LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	bool Init() override;
	bool Frame() override;
	bool Render(ID3D11DeviceContext* pContext) override;
	bool Release() override;

public:
	HModel() = default;
	virtual ~HModel() = default;

};