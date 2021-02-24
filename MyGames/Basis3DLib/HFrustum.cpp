#include "HFrustum.h"
bool HFrustum::Create(ID3D11Device* pd3dDevice)
{
	if (!m_FrustumObj.Create(pd3dDevice, L"VS.txt", L"PS.txt", L""))
	{
		return false;
	}

	for (auto v : m_FrustumObj.m_VertexList)
	{
		m_VertexList.push_back(v);
	}

	m_VertexList[0].p = Vector3(-1.0f, 1.0f, 0.0f);
	m_VertexList[1].p = Vector3(1.0f, 1.0f, 0.0f);
	m_VertexList[2].p = Vector3(1.0f, -1.0f, 0.0f);
	m_VertexList[3].p = Vector3(-1.0f, -1.0f, 0.0f);

	m_VertexList[8].p = Vector3(1.0f, 1.0f, 0.0f);
	m_VertexList[11].p = Vector3(1.0f, -1.0f, 0.0f);

	m_VertexList[13].p = Vector3(-1.0f, 1.0f, 0.0f);
	m_VertexList[14].p = Vector3(-1.0f, -1.0f, 0.0f);

	m_VertexList[18].p = Vector3(1.0f, 1.0f, 0.0f);
	m_VertexList[19].p = Vector3(-1.0f, 1.0f, 0.0f);

	m_VertexList[20].p = Vector3(-1.0f, -1.0f, 0.0f);
	m_VertexList[21].p = Vector3(1.0f, -1.0f, 0.0f);

	m_Plane.resize(6);

	return true;
}

bool HFrustum::Frame()
{
	// 초기화 때 한번 호출되므로 예외처리.
	if (m_Plane.size() <= 0) return true;
	m_Plane[0].Create(
		m_FrustumObj.m_VertexList[2].p,
		m_FrustumObj.m_VertexList[1].p,
		m_FrustumObj.m_VertexList[0].p);
	m_Plane[1].Create(
		m_FrustumObj.m_VertexList[6].p,
		m_FrustumObj.m_VertexList[5].p,
		m_FrustumObj.m_VertexList[4].p);
	m_Plane[2].Create(
		m_FrustumObj.m_VertexList[10].p,
		m_FrustumObj.m_VertexList[9].p,
		m_FrustumObj.m_VertexList[8].p);
	m_Plane[3].Create(
		m_FrustumObj.m_VertexList[14].p,
		m_FrustumObj.m_VertexList[13].p,
		m_FrustumObj.m_VertexList[12].p);
	m_Plane[4].Create(
		m_FrustumObj.m_VertexList[18].p,
		m_FrustumObj.m_VertexList[17].p,
		m_FrustumObj.m_VertexList[16].p);
	m_Plane[5].Create(
		m_FrustumObj.m_VertexList[22].p,
		m_FrustumObj.m_VertexList[21].p,
		m_FrustumObj.m_VertexList[20].p);

	return true;
}

BOOL HFrustum::ClassifyPoint(Vector3 v)
{
	for (int iPlane = 0; iPlane < m_Plane.size(); iPlane++)
	{
		float fDist = m_Plane[iPlane].a * v.x +
					  m_Plane[iPlane].b * v.y +
					  m_Plane[iPlane].c * v.z +
					  m_Plane[iPlane].d;

		if (fDist < -1.0f)
		{
			return false;
		}
	}

	return true;
}

BOOL HFrustum::ClassifyBox(HShapeBox box)
{
	return 0;
}
