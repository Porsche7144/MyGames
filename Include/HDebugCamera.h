#pragma once
#include "HCamera.h"

class HDebugCamera : public HCamera
{
public:
	Vector4 m_vDirValue;
	POINT	m_ptPrePosition;

public:
	void Update(Vector4 data);
	bool Init();
	bool Frame();

public:
	HDebugCamera();
	virtual ~HDebugCamera();
};

