#include "HSelect.h"

bool HSelect::IntersectBox(H_BOX* pBox, H_RAY* pRay)
{
	Vector3 dir = pRay->vDir;
	Vector3 tMin, tMax;

	// 0.0001f로 나누는 이유는 0으로 나눠지는 것을 방지하기 위해.
	tMin.x = (pBox->vMin.x - pRay->vOrigin.x) / (dir.x + 0.0001f);
	tMin.y = (pBox->vMin.y - pRay->vOrigin.y) / (dir.y + 0.0001f);
	tMin.z = (pBox->vMin.z - pRay->vOrigin.z) / (dir.z + 0.0001f);

	tMax.x = (pBox->vMax.x - pRay->vOrigin.x) / (dir.x + 0.0001f);
	tMax.y = (pBox->vMax.y - pRay->vOrigin.y) / (dir.y + 0.0001f);
	tMax.z = (pBox->vMax.z - pRay->vOrigin.z) / (dir.z + 0.0001f);

	Vector3 real_min, real_max;
	// min(a,b)            (((a) < (b)) ? (a) : (b))
	real_min.x = min(tMin.x, tMax.x);
	real_min.y = min(tMin.y, tMax.y);
	real_min.z = min(tMin.z, tMax.z);

	// max(a,b)            (((a) > (b)) ? (a) : (b))
	real_max.x = max(tMin.x, tMax.x);
	real_max.y = max(tMin.y, tMax.y);
	real_max.z = max(tMin.z, tMax.z);

	float minmax = min(min(real_max.x, real_max.y), real_max.z);
	float maxmin = max(max(real_min.x, real_min.y), real_min.z);

	if (minmax >= maxmin)
	{
		m_vIntersection = pRay->vOrigin + pRay->vDir * maxmin;
		return true;
	}

	return false;

}

bool HSelect::IntersectRayToSphere(H_SPHERE* pSphere, H_RAY* pRay)
{
	if (pRay == NULL) pRay = &m_Picking.m_Ray;

	// p0 - c
	Vector3 vDir = pRay->vOrigin - pSphere->vCenter;
	// B = 2(u dot (p0 - c ))
	float  fProj = vDir.Dot(pRay->vDir);
	// 제한된 길이(세그먼트) 제외처리
	if (pRay->fExtent > -1.0f && pRay->fExtent < fabs(fProj)) return false;

	float b = 2.0f * fProj;
	// C= (p0-c) dot (p0-c) - r*r
	float c = vDir.Dot(vDir) - (pSphere->fRadius * pSphere->fRadius);
	// D = B*B - 4*AC;
	float fDiscriminant = (b*b) - (4.0f*c);
	if (fDiscriminant < 0.0f)
	{
		return false;
	}
	fDiscriminant = sqrtf(fDiscriminant);
	float t0 = (-b + fDiscriminant) / 2.0f;
	float t1 = (-b - fDiscriminant) / 2.0f;

	if (t0 >= 0.0f)
	{
		m_vIntersection = pRay->vOrigin + pRay->vDir * t0;
		return true;
	}
	if (t1 >= 0.0f)
	{
		m_vIntersection = pRay->vOrigin + pRay->vDir * t1;
		return true;
	}
	return false;
}

