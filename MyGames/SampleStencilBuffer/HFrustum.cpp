#include "HFrustum.h"
// OBB 제외처리( 프로스텀 내부에 존재( 걸쳐 있어도)하면 TRUE, 아니면 FALSE )

BOOL HFrustum::CheckOBBInPlane(H_BOX*  pBox)
{
	float		fPlaneToCenter = 0.0;
	float		fDistance = 0.0f;
	Vector3 vDir;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		vDir = pBox->vAxis[0] * pBox->fExtent[0];
		fDistance = fabs(m_Plane[iPlane].a * vDir.x + m_Plane[iPlane].b*vDir.y + m_Plane[iPlane].c * vDir.z);
		vDir = pBox->vAxis[1] * pBox->fExtent[1];
		fDistance += fabs(m_Plane[iPlane].a * vDir.x + m_Plane[iPlane].b*vDir.y + m_Plane[iPlane].c * vDir.z);
		vDir = pBox->vAxis[2] * pBox->fExtent[2];
		fDistance += fabs(m_Plane[iPlane].a * vDir.x + m_Plane[iPlane].b*vDir.y + m_Plane[iPlane].c * vDir.z);

		fPlaneToCenter = m_Plane[iPlane].a * pBox->vCenter.x + m_Plane[iPlane].b*pBox->vCenter.y +
			m_Plane[iPlane].c * pBox->vCenter.z + m_Plane[iPlane].d;

		if (fPlaneToCenter <= -fDistance)
		{
			return FALSE;
		}
	}
	return TRUE;
}

bool HFrustum::Create(ID3D11Device* pd3dDevice)
{
	if (!m_FrustumObj.Create(pd3dDevice, L"vs.txt",
		L"ps.txt",
		L""))
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
		if (fDist < -1.1f)
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
