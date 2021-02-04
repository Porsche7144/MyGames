#pragma once
#include "HStd.h"

struct HVertex3
{
	float x;
	float y;
	float z;

	HVertex3() {};
	HVertex3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};