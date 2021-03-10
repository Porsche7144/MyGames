#pragma once
#include "HCore.h"
#include "HShape.h"
#include "HObject.h"
#include "HheightMap.h"
#include "HMinimap.h"
#include "HQuadTree.h"
#include "HPicking.h"
#include "HModelViewCamera.h"
#include "HSelect.h"

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
	HSelect			m_Select;
	HQuadTree		m_QuadTree;
	H_BOX			m_HBox;
	HPicking		m_Picking;

	HShapeBox			m_ShapeBox;
	HheightMap			m_Map;
	HMinimap			m_Minimap;
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

	std::vector<HObject*> m_ObjList;



public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool PostRender() override;
	bool Release() override;

public:
	void SetRasterState();
	LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	Sample() 
	{
		t, u, v = 0;
	};
	~Sample() {};

};

HGAME_RUN;