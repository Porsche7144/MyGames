#pragma once
#include "HStd.h"

struct HVector2
{
	float x;
	float y;

	HVector2() {};
	HVector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

struct HVector3
{
	float x;
	float y;
	float z;

	HVector3() {};
	HVector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

struct HVector4
{
	float x;
	float y;
	float z;
	float w;

	HVector4() {};
	HVector4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
};