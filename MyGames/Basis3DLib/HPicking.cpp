#include "HPicking.h"

bool HPicking::GetIntersection(Vector3 vStart, Vector3 vEnd, Vector3 vNomal, Vector3 v0, Vector3 v1, Vector3 v2)
{
	Vector3 vDirection = vEnd - vStart;
	float D = vNomal.Dot(vDirection);
	float a0 = vNomal.Dot((v0 - vStart));
	float fT = a0 / D;

	if (fT < 0.0f || fT > 1.0f)
	{
		return false;
	}

	m_vInterSection = vStart + vDirection * fT;

	return true;
}

// ������ �̿��ϴ� ���
bool HPicking::PointInPolygon(Vector3 vert, Vector3 faceNormal, Vector3 v0, Vector3 v1, Vector3 v2)
{
	Vector3 e0, e1, e2, ilnter, vNormal;

	e0 = v1 - v0;
	e1 = v2 - v1;
	e2 = v0 - v2;

	ilnter = vert - v0;
	vNormal = e0.Cross(ilnter);
	vNormal.Normalize();
	float fDot = faceNormal.Dot(vNormal);
	// ������ �ﰢ�� �ȿ� �������� ����.
	if (fDot < 0.0f) return false;

	ilnter = vert - v1;
	vNormal = e1.Cross(ilnter);
	vNormal.Normalize();
	fDot = faceNormal.Dot(vNormal);
	if (fDot < 0.0f) return false;

	ilnter = vert - v2;
	vNormal = e2.Cross(ilnter);
	vNormal.Normalize();
	fDot = faceNormal.Dot(vNormal);
	if (fDot < 0.0f) return false;

	return true;
}

// UV �Ű������� ����� ������ ���� ���� �׽�Ʈ ���ÿ� ó�� �ϴ� ���(��� �븻 �ʿ�X).
bool HPicking::IntersectTriangle(Vector3 Origin, Vector3 Dir, Vector3 v0, Vector3 v1, Vector3 v2, float t, float u, float v)
{
	Vector3 edge1 = v1 - v0;
	Vector3 edge2 = v2 - v0;

	// ��Ľ� ���
	Vector3 Pvec = Dir.Cross(edge2);
	float det = edge1.Dot(Pvec);

	Vector3 Tvec;

	if (det > 0)
	{
		Tvec = Origin - v0;
	}
	else
	{
		Tvec = v0 - Origin;
		det = -det;
	}
	if (det < 0) return false;


	// u ���
	u = Tvec.Dot(Pvec);
	if (u < 0 || u > det) return false;

	// v ���
	Vector3 Qvec = Tvec.Cross(edge1);
	v = Dir.Dot(Qvec);
	if (v < 0 || u + v > det) return false;

	// t ���
	t = edge2.Dot(Qvec);

	// ��� ����
	float fInvDet = 1.0f / det;
	t *= fInvDet;
	u *= fInvDet;
	v *= fInvDet;

	return true;
}

bool HPicking::IntersectBox(H_BOX* pBox, H_RAY* pRay)
{
	if (pRay == NULL) pRay = &m_Ray;

	Vector3 dir = pRay->vDir;
	Vector3 tMin, tMax;

	// 0.0001f�� ������ ������ 0���� �������� ���� �����ϱ� ����.
	tMin.x = (pBox->vMin.x - pRay->vOrigin.x) / (dir.x + 0.001f);
	tMin.y = (pBox->vMin.y - pRay->vOrigin.y) / (dir.y + 0.0001f);
	tMin.z = (pBox->vMin.z - pRay->vOrigin.z) / (dir.z + 0.0001f);

	tMax.x = (pBox->vMax.x - pRay->vOrigin.x) / (dir.x + 0.001f);
	tMax.y = (pBox->vMax.y - pRay->vOrigin.y) / (dir.y + 0.001f);
	tMax.z = (pBox->vMax.z - pRay->vOrigin.z) / (dir.z + 0.001f);

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
		m_vInterSection = pRay->vOrigin + dir * maxmin;
		return true;
	}

	return false;

}
