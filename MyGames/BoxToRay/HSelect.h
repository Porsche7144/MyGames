#pragma once
#include "HShape.h"
#include "HPicking.h"

class HSelect
{
public:
	HPicking m_Picking;
	Vector3 m_vIntersection;

public:
	Matrix						m_matWorld;
	Matrix						m_matView;
	Matrix						m_matProj;
	Vector3						m_vCenter;

public:
	bool IntersectBox(H_BOX* pBox, H_RAY* pRay);
};

