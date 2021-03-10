#pragma once
#include "HCore.h"
#include "HShape.h"
#include "HObject.h"
#include "HheightMap.h"
#include "HMinimap.h"
#include "HQuadTree.h"
#include "HPicking.h"
#include "HModelViewCamera.h"

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
	HQuadTree		m_QuadTree;
	H_BOX			m_TBoxBase;
	HPicking		m_Picking;

	HMap			m_Map;
	HMinimap			m_Minimap;
	HBoxUser			m_BoxShape;
	HBoxUser			m_UserShape;
	HShapePlane			m_PlaneShape;
	Vector4				m_vDirValue;
	HModelViewCamera	m_ModelCamera;
	HCamera m_TopCamera;

	Vector3 vPickRayDir;
	Vector3 vPickRayOrigin;

	Vector3 v0, v1, v2, vNormal, vEnd;
	Vector3 list[3];

	float t, u, v;
	bool m_bSelect;

	std::vector<HObject*> m_ObjList;
	std::vector<HNode*> m_SelectNode;

public:

	Matrix						m_matBoxWorld;
	Matrix						m_matPlaneWorld;
	Matrix						m_matLineWorld;

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool PostRender() override;
	bool Release() override;

public:
	void SetRasterState();
	bool GetIntersection(HNode* pNode);
	LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	Sample() 
	{
		t, u, v = 0;
	};
	~Sample() {};

};

HGAME_RUN;