#include "HShape.h"

HShape::HShape()
{
}

HShape::~HShape()
{
}


bool HShapeBox::CreateVertexData()
{
	// Vertex Data
	m_VertexList.resize(24);
	m_VertexList[0] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(Vector3(1.0f, 1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[2] = PNCT_VERTEX(Vector3(1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[3] = PNCT_VERTEX(Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f));
	// 뒷면
	m_VertexList[4] = PNCT_VERTEX(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector4(0.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[5] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[6] = PNCT_VERTEX(Vector3(-1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[7] = PNCT_VERTEX(Vector3(1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f));

	// 오른쪽
	m_VertexList[8] = PNCT_VERTEX(Vector3(1.0f, 1.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[9] = PNCT_VERTEX(Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[10] = PNCT_VERTEX(Vector3(1.0f, -1.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[11] = PNCT_VERTEX(Vector3(1.0f, -1.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f));

	// 왼쪽
	m_VertexList[12] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[13] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, -1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[14] = PNCT_VERTEX(Vector3(-1.0f, -1.0f, -1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[15] = PNCT_VERTEX(Vector3(-1.0f, -1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f));

	// 윗면
	m_VertexList[16] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 0.5f, 1.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[17] = PNCT_VERTEX(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 0.5f, 1.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[18] = PNCT_VERTEX(Vector3(1.0f, 1.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 0.5f, 1.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[19] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 0.5f, 1.0f, 1.0f), Vector2(0.0f, 1.0f));

	// 아랫면
	m_VertexList[20] = PNCT_VERTEX(Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[21] = PNCT_VERTEX(Vector3(1.0f, -1.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[22] = PNCT_VERTEX(Vector3(1.0f, -1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[23] = PNCT_VERTEX(Vector3(-1.0f, -1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f));
	return true;

	return true;
}

bool HShapeBox::CreateIndexData()
{
	m_IndexList.resize(36);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; 	m_IndexList[iIndex++] = 1; 	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 0;	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 3;
	m_IndexList[iIndex++] = 4; 	m_IndexList[iIndex++] = 5; 	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 4;	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 7;
	m_IndexList[iIndex++] = 8; 	m_IndexList[iIndex++] = 9; 	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 8;	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 11;
	m_IndexList[iIndex++] = 12; m_IndexList[iIndex++] = 13; m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 12;	m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 15;
	m_IndexList[iIndex++] = 16; m_IndexList[iIndex++] = 17; m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 16;	m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 19;
	m_IndexList[iIndex++] = 20; m_IndexList[iIndex++] = 21; m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 20;	m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 23;
	return true;

	return true;
}

HShapeBox::HShapeBox()
{
}

HShapeBox::~HShapeBox()
{
}

bool HShapePlane::CreateVertexData()
{
	// Vertex Data
	m_VertexList.resize(4);
	m_VertexList[0] = PNCT_VERTEX(Vector3(-1.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(Vector3(1.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f));
	m_VertexList[2] = PNCT_VERTEX(Vector3(1.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f));
	m_VertexList[3] = PNCT_VERTEX(Vector3(-1.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f));
	return true;

	return true;
}

bool HShapePlane::CreateIndexData()
{
	m_IndexList.resize(6);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0;
	m_IndexList[iIndex++] = 1;
	m_IndexList[iIndex++] = 2;
	m_IndexList[iIndex++] = 0;
	m_IndexList[iIndex++] = 2;
	m_IndexList[iIndex++] = 3;

	return true;
}

HShapePlane::HShapePlane()
{
}

HShapePlane::~HShapePlane()
{
}

bool HShapeLine::Draw(ID3D11DeviceContext * pContext, Vector3 p, Vector3 e, Vector4 c)
{
	m_VertexList[0] = PNCT_VERTEX(p, Vector3(0.0f, 0.0f, -1.0f), c, Vector2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(e, Vector3(0.0f, 0.0f, -1.0f), c, Vector2(1.0f, 0.0f));
	pContext->UpdateSubresource(
		m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);

	return Render(pContext);
}

bool HShapeLine::CreateVertexData()
{
	// Vertex Data
	m_VertexList.resize(2);
	m_VertexList[0] = PNCT_VERTEX(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(Vector3(100.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f));
	return true;
}

bool HShapeLine::CreateIndexData()
{
	m_IndexList.resize(2);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0;
	m_IndexList[iIndex++] = 1;
	return true;
}

HShapeLine::HShapeLine()
{
	m_szPixelShader = "PSLine";
	m_iTopology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
}

HShapeLine::~HShapeLine()
{
}

bool HShapeSphere::CreateVertexData()
{
	CreateSphere(40, 1.0f);

	return true;
}

bool HShapeSphere::CreateIndexData()
{

	return true;
}

void HShapeSphere::MakeSpherePoint(std::vector<Vector3> &spherePoints, UINT Slices,	double fRadius, Vector3 center)
{

	spherePoints.clear();
	spherePoints.resize(Slices* Slices);

	for (DWORD j = 0; j < Slices; j++)
	{
		float theta = (float)((XM_PI*j) / (Slices));
		for (DWORD i = 0; i < Slices; i++)
		{
			int  iPos = j * Slices + i;
			float phi = (2 * XM_PI*i) / (Slices);
			spherePoints[iPos].x = (float)fRadius * (float)(sin(theta)*cos(phi)) + center.x;
			spherePoints[iPos].z = (float)fRadius * (float)(sin(theta)*sin(phi)) + center.y;
			spherePoints[iPos].y = (float)fRadius * (float)(cos(theta)) + center.z;
		}
	}
}


bool HShapeSphere::Draw(ID3D11DeviceContext * pContext)
{
	CreateSphere(40, 1.0f);
	pContext->UpdateSubresource(
		m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);

	return Render(pContext);
}

void HShapeSphere::CreateSphere(UINT Slices, float fRadius)
{
	const double PI = 3.141592653589793238462643383279502884197;
	int iPos = 0;
	vector<Vector3> VertexArray;
	MakeSpherePoint(VertexArray, Slices, fRadius, Vector3(0, 0, 0));

	//m_VertexList.resize(VertexArray.size());

	int iNext = 0;
	for (DWORD j = 0; j < Slices; j++)
	{
		for (DWORD i = 0; i < Slices; i++)
		{
			if (i == Slices - 1)         iNext = 0;
			else iNext = i + 1;
			PNCT_VERTEX v0, v1, v2, v3, v4, v5;
			iPos = (j*Slices * 6) + (i * 6);

			v0.p = VertexArray[j*Slices + i];
			v1.p = VertexArray[j*Slices + iNext];

			if (j != Slices - 1) v2.p = VertexArray[((j + 1)*Slices) + i];
			else                v2.p = Vector3(0, -1, 0);

			v3.p = v2.p;
			v4.p = v1.p;

			if (j != Slices - 1) v5.p = VertexArray[((j + 1)*Slices) + iNext];
			else                 v5.p = Vector3(0, -1, 0);

			v0.p.Normalize(v0.n);
			v1.p.Normalize(v1.n);
			v2.p.Normalize(v2.n);
			v3.p.Normalize(v3.n);
			v4.p.Normalize(v4.n);
			v5.p.Normalize(v5.n);

			v0.c = Vector4(1, 1, 1, 1.0f); //TVector4(v0.n.x, v0.n.y, v0.n.z, 1.0f);
			v1.c = Vector4(1, 1, 1, 1.0f); //TVector4(v1.n.x, v1.n.y, v1.n.z, 1.0f);
			v2.c = Vector4(1, 1, 1, 1.0f); //TVector4(v2.n.x, v2.n.y, v2.n.z, 1.0f);
			v3.c = Vector4(1, 1, 1, 1.0f); //TVector4(v3.n.x, v3.n.y, v3.n.z, 1.0f);
			v4.c = Vector4(1, 1, 1, 1.0f); //TVector4(v4.n.x, v4.n.y, v4.n.z, 1.0f);
			v5.c = Vector4(1, 1, 1, 1.0f); //TVector4(v5.n.x, v5.n.y, v5.n.z, 1.0f);

			// 구 텍스처 좌표
			v0.t = Vector2((FLOAT)(atan2(v0.n.z, v0.n.x) / (PI*2.0f) + 0.5f), (FLOAT)(asinf(-v0.n.y) / PI + 0.5f));
			v1.t = Vector2((FLOAT)(atan2(v1.n.z, v1.n.x) / (PI*2.0f) + 0.5f), (FLOAT)(asinf(-v1.n.y) / PI + 0.5f));
			v2.t = Vector2((FLOAT)(atan2(v2.n.z, v2.n.x) / (PI*2.0f) + 0.5f), (FLOAT)(asinf(-v2.n.y) / PI + 0.5f));
			v3.t = Vector2((FLOAT)(atan2(v3.n.z, v3.n.x) / (PI*2.0f) + 0.5f), (FLOAT)(asinf(-v3.n.y) / PI + 0.5f));
			v4.t = Vector2((FLOAT)(atan2(v4.n.z, v4.n.x) / (PI*2.0f) + 0.5f), (FLOAT)(asinf(-v4.n.y) / PI + 0.5f));
			v5.t = Vector2((FLOAT)(atan2(v5.n.z, v5.n.x) / (PI*2.0f) + 0.5f), (FLOAT)(asinf(-v5.n.y) / PI + 0.5f));

			//////////////////////////////////////
			//   V0.t.x( 0.94f) ---- V1.t.x(0.0f) 일 경우에 
			//   v1.t.x= 1.0f으로 조정한다.
			//////////////////////////////////////
			if (fabs(v1.t.x - v0.t.x) > 0.9f)
			{
				if (fabs(v1.t.x) <= 0.01f)
				{
					v1.t.x = 1.0f;
				}
				if (fabs(v0.t.x) <= 0.01f)
				{
					v0.t.x = 1.0f;
				}
			}
			if (fabs(v2.t.x - v0.t.x) > 0.9f)
			{
				if (fabs(v2.t.x) <= 0.01f)
				{
					v2.t.x = 1.0f;
				}
				if (fabs(v0.t.x) <= 0.01f)
				{
					v0.t.x = 1.0f;
				}
			}
			if (fabs(v4.t.x - v3.t.x) > 0.9f)
			{
				if (fabs(v4.t.x) <= 0.01f)
				{
					v4.t.x = 1.0f;
				}
				if (fabs(v3.t.x) <= 0.01f)
				{
					v3.t.x = 1.0f;
				}
			}
			if (fabs(v5.t.x - v3.t.x) > 0.9f)
			{
				if (fabs(v5.t.x) <= 0.01f)
				{
					v5.t.x = 1.0f;
				}
				if (fabs(v3.t.x) <= 0.01f)
				{
					v3.t.x = 1.0f;
				}
			}
			m_VertexList.push_back(v0);
			m_VertexList.push_back(v1);
			m_VertexList.push_back(v2);
			m_VertexList.push_back(v3);
			m_VertexList.push_back(v4);
			m_VertexList.push_back(v5);
		}
	}
	VertexArray.clear();
}

HShapeSphere::HShapeSphere()
{
}

HShapeSphere::~HShapeSphere()
{
}
