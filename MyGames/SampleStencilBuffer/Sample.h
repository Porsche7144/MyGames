#pragma once
#include "HCore.h"
#include "HShape.h"
#include "HDebugCamera.h"

#pragma comment(lib, "directxtk.lib")

class Sample : public HCore
{
public:
	HShapeBox m_Box;
	HShapePlane m_Plane;
	HShapeLine m_Line;
	HDebugCamera m_Camera;
	Vector4 m_vDirValue;

public:

	Matrix						m_matBoxWorld;
	Matrix						m_matPlaneWorld;
	Matrix						m_matLineWorld;

public:
	Matrix* TD3DXMatrixShadow(Matrix *pout,
		Vector4 *plight,
		Vector4 *pplane);
	Matrix CreateMatrixShadow(
		Vector4* pPlane,
		Vector4* pLight);
	LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;

public:
	void SetRasterState();

};

HGAME_RUN;