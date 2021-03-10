#pragma once
#include "HMap.h"

struct H_RAY
{
	float fExtent;
	Vector3 vOrigin;
	Vector3 vDir;
	H_RAY()
	{
		// fExtent > 0;  세그먼트
		fExtent = -1;
	}
};

class HPicking
{
public:
	H_RAY m_Ray;
	Vector3 m_vInterSection;

public:
	bool GetIntersection(Vector3 vStart, Vector3 vEnd, Vector3 vNomal, Vector3 v0, Vector3 v1, Vector3 v2);
	bool PointInPolygon(Vector3 vert, Vector3 faceNomal, Vector3 v0, Vector3 v1, Vector3 v2);
	bool IntersectTriangle(Vector3 Origin, Vector3 Dir, Vector3 v0, Vector3 v1, Vector3 v2, float t, float u, float v);
	bool IntersectBox(H_BOX* pBox, H_RAY* pRay);

};