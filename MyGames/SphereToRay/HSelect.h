#pragma once
#include "HShape.h"

struct H_Ray
{
	float fExtent;
	Vector3 vOrigin;
	Vector3 vDir;
	H_Ray()
	{
		// fExtent > 0;  세그먼트
		fExtent = -1;
	}
};

class HSelect
{
public:
	H_Ray m_Ray;
	Vector3 m_vIntersection;

public:
	Matrix						m_matWorld;
	Matrix						m_matView;
	Matrix						m_matProj;
	Vector3						m_vCenter;

public:
	bool IntersectBox(H_BOX* pBox, H_Ray* pRay);
};

