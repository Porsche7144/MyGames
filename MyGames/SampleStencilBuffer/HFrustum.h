#pragma once
#include "HStd.h"
#include "HShape.h"

struct HPlane
{
	float a, b, c, d;
	void Create(Vector3 v0, Vector3 v1, Vector3 v2)
	{
		Vector3 n;
		Vector3 e0 = v1 - v0;
		Vector3 e1 = v2 - v0;
		n = e0.Cross(e1);
		n.Normalize();
		a = n.x;
		b = n.y;
		c = n.z;
		d = -n.Dot(v0);
	}
	void Create(Vector3 n, Vector3 v)
	{
		n.Normalize();
		a = n.x;
		b = n.y;
		c = n.z;
		d = -n.Dot(v);
	}
};
class HFrustum
{
public:
	HShapeBox  m_FrustumObj;
	std::vector<PNCT_VERTEX>	m_VertexList;
	std::vector<HPlane>   m_Plane;
public:
	virtual bool Create(ID3D11Device* pd3dDevice);
	virtual bool Frame();

	BOOL	ClassifyPoint(Vector3 v);
	BOOL	ClassifyBox(HShapeBox box);
	BOOL    CheckOBBInPlane(H_BOX* pBox);
};

