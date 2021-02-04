#pragma once
#include "HStd.h"

struct HVertex2
{
	float x;
	float y;

	HVertex2() {};
	HVertex2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

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

struct HVertex4
{
	float x;
	float y;
	float z;
	float w;

	HVertex4() {};
	HVertex4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
};