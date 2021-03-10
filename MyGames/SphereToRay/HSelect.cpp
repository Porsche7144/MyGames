#include "HSelect.h"

bool HSelect::IntersectBox(H_BOX* pBox, H_Ray* pRay)
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

