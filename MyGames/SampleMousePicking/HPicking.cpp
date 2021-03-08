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

// UV 매개변수를 사용한 교점과 교점 포함 테스트 동시에 처리 하는 방법(평면 노말 필요X).
bool HPicking::IntersectTriangle(Vector3 Origin, Vector3 Dir, Vector3 v0, Vector3 v1, Vector3 v2, float t, float u, float v)
{
	Vector3 edge1 = v1 - v0;
	Vector3 edge2 = v2 - v0;

	// 행렬식 계산
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


	// u 계산
	u = Tvec.Dot(Pvec);
	if (u < 0 || u > det) return false;

	// v 계산
	Vector3 Qvec = Tvec.Cross(edge1);
	v = Dir.Dot(Qvec);
	if (v < 0 || u + v > det) return false;

	// t 계산
	t = edge2.Dot(Qvec);

	// 행렬 곱셈
	float fInvDet = 1.0f / det;
	t *= fInvDet;
	u *= fInvDet;
	v *= fInvDet;

	return true;
}