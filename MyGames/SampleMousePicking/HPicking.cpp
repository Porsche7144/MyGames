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

// 외적을 이용하는 방법
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
	// 교점이 삼각형 안에 존재하지 않음.
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
