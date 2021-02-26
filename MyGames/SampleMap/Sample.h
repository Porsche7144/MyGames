#pragma once
#include "HCore.h"
#include "HShape.h"
#include "HObject.h"
#include "HModelViewCamera.h"
#include "HheightMap.h"
#include "HMinimap.h"
#include "HQuadTree.h"

#pragma comment(lib, "directxtk.lib")

class HBoxUser : public HShapeBox
{
public:
	void FrontMovement(float fDir);
	void RightMovement(float fDir);
	void UpMovement(float fDir);
	void RightBase(float fDir);
	void FrontBase(float fDir);
	void UpBase(float fDir);
};

class Sample : public HCore
{
public:
	HheightMap m_Map;
	HMinimap m_Minimap;
	HQuadTree m_QuadTree;
	HBoxUser			m_BoxShape;
	HShapePlane			m_PlaneShape;
	Vector4				m_vDirValue;
	HModelViewCamera	m_ModelCamera;

	HCamera m_TopCamera;

	std::vector<HObject*> m_ObjList;

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
	bool DrawQuadLine(HNode* pNode);

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool PostRender() override;
	bool Release() override;

public:
	void SetRasterState();

};

HGAME_RUN;