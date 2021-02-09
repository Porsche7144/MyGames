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
	HVector3					m_vCameraPos = { -5,-3,0 };
	HVector3					m_vCameraTarget = { 0,0,0 };

	HMatrix						m_matBoxWorld;
	HMatrix						m_matPlaneWorld;
	HMatrix						m_matLineWorld;

	HMatrix						m_matView;
	HMatrix						m_matProject;

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;

public:
	void CompilerCheck(ID3DBlob* ErrorMsg);
	void SetRasterState();

};

HGAME_RUN;