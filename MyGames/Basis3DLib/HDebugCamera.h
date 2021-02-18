#pragma once
#include "HCamera.h"

class HDebugCamera : public HCamera
{
public:
	void Update(Vector4 data);
	bool Init();
	bool Frame();

public:
	HDebugCamera();
	virtual ~HDebugCamera();
};

