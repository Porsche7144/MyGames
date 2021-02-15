#pragma once
#include "HCamera.h"

class HDebugCamera : public HCamera
{
public:
	Vector4 m_vDirValue;

public:
	void Update(Vector4 data);
	bool Frame();

public:
	HDebugCamera();
	virtual ~HDebugCamera();
};

