#pragma once
#include "HCore.h"
#include "HShape.h"

#pragma comment(lib, "directxtk.lib")

class Sample : public HCore
{
public:
	HShapeBox m_Box;
	HShapePlane m_Plane;
	HShapeLine m_Line;

public:
	HVector3					m_vCameraPos = { 10,0,-10 };
	HVector3					m_vCameraTarget = { 0,0,0 };

	HMatrix						m_matBoxWorld;
	HMatrix						m_matPlaneWorld;
	HMatrix						m_matLineWorld;

	HMatrix						m_matView;
	HMatrix						m_matProject;

public:
	HMatrix* TD3DXMatrixShadow(HMatrix *pout,
		HVector4 *plight,
		HVector4 *pplane);
	HMatrix CreateMatrixShadow(
		HVector4* pPlane,
		HVector4* pLight);

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;

public:
	void SetRasterState();

};

HGAME_RUN;