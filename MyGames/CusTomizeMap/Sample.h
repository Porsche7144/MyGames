#pragma once
#include "HCore.h"
#include "HMap.h"

class Sample : public HCore
{
public:
	HMap m_CustomMap;
	HCamera* m_MainCamera;
	HShapeLine m_LineDraw;

	Matrix						m_matBoxWorld;
	Matrix						m_matPlaneWorld;
	Matrix						m_matLineWorld;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	Sample();
	virtual ~Sample();
};
HGAME_RUN;