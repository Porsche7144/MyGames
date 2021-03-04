#include "HQuadTree.h"


 // 0 31 63

bool HQuadTree::Build(HMap& map)
{
	float Width = map.m_iNumCols -1;
	float Height = map.m_iNumRows -1;

	m_pParentNode = CreateTreeNode(-Width / 2, Width / 2, -Height / 2, Height / 2);
	m_pParentNode->m_dwDepth = 0;

	if (SetTree(m_pParentNode))
	{
		return true;
	}

	return false;
}

bool HQuadTree::SetTree(HNode* pNode)
{
	if (NodeDivide(pNode))
	{
		for (int node = 0; node < pNode->m_ChildList.size(); node++)
		{
			SetTree(pNode->m_ChildList[node]);
		}
		return true;
	}

	return false;
}

bool HQuadTree::Render(ID3D11DeviceContext * pContext)
{


	return true;
}

bool HQuadTree::NodeDivide(HNode* pNode)
{
	if (pNode == NULL)
	{
		return false;
	}

	// 노드의 절반크기
	float fHalfWidth = (pNode->m_CornerList[TOP_RIGHT].x - pNode->m_CornerList[TOP_LEFT].x) / 2;
	float fHalfHeight = -(pNode->m_CornerList[BOTTOM_LEFT].z - pNode->m_CornerList[TOP_LEFT].z) / 2;


	// 노드가 m_DivideSize의 크기보다 작다면 분할하지 않음.
	if (fHalfWidth < m_DivideSize || fHalfHeight < m_DivideSize)
	{
		return false;
	}

	// 0  1  e0  3  4
	// 5  6  7  8   9 
	// e1 11 e2 13 e3
	// 15 16 17 18 19 
	// 20 21 e4 23 24


	//            |
	//            |
	//            |
	// ------------------------
	//            |
	//            |
	//            |


	// ---------
	//			|
	//			|
	m_ltPos[0].TopLeft = pNode->m_CornerList[TOP_LEFT].x;
	m_ltPos[0].TopRight = pNode->m_CornerList[TOP_LEFT].x + fHalfWidth;
	m_ltPos[0].BottomLeft = pNode->m_CornerList[TOP_LEFT].z - fHalfHeight;
	m_ltPos[0].BottomeRight = pNode->m_CornerList[TOP_LEFT].z;


	//  ---------
	// |
	// |
	m_ltPos[1].TopLeft = pNode->m_CornerList[TOP_LEFT].x + fHalfWidth;
	m_ltPos[1].TopRight = pNode->m_CornerList[TOP_RIGHT].x;
	m_ltPos[1].BottomLeft = pNode->m_CornerList[TOP_LEFT].z - fHalfHeight;
	m_ltPos[1].BottomeRight = pNode->m_CornerList[TOP_LEFT].z;

	//			|
	//			|
	//  --------
	m_ltPos[2].TopLeft = pNode->m_CornerList[TOP_LEFT].x;
	m_ltPos[2].TopRight = pNode->m_CornerList[TOP_LEFT].x + fHalfWidth;
	m_ltPos[2].BottomLeft = pNode->m_CornerList[BOTTOM_LEFT].z;
	m_ltPos[2].BottomeRight = pNode->m_CornerList[TOP_LEFT].z - fHalfHeight;

	// |
	// |
	//  ---------
	m_ltPos[3].TopLeft = pNode->m_CornerList[TOP_LEFT].x + fHalfWidth;
	m_ltPos[3].TopRight = pNode->m_CornerList[TOP_RIGHT].x;
	m_ltPos[3].BottomLeft = pNode->m_CornerList[BOTTOM_RIGHT].z;
	m_ltPos[3].BottomeRight = pNode->m_CornerList[TOP_LEFT].z - fHalfHeight;


	pNode->m_ChildList.push_back(CreateTreeNode(
			m_ltPos[0].TopLeft, m_ltPos[0].TopRight, m_ltPos[0].BottomLeft, m_ltPos[0].BottomeRight));

	pNode->m_ChildList.push_back(CreateTreeNode(
		m_ltPos[1].TopLeft, m_ltPos[1].TopRight, m_ltPos[1].BottomLeft, m_ltPos[1].BottomeRight));

	pNode->m_ChildList.push_back(CreateTreeNode(
		m_ltPos[2].TopLeft, m_ltPos[2].TopRight, m_ltPos[2].BottomLeft, m_ltPos[2].BottomeRight));

	pNode->m_ChildList.push_back(CreateTreeNode(
		m_ltPos[3].TopLeft, m_ltPos[3].TopRight, m_ltPos[3].BottomLeft, m_ltPos[3].BottomeRight));


	return true;
}

HNode* HQuadTree::CreateTreeNode(float topLeft, float topRight, float bottomLeft, float bottomRight)
{
	HNode* pNode = new HNode;

	pNode->m_ChildList.resize(4);
	pNode->m_CornerList.resize(4);

	float vCenter = 0.0f;
	Vector3 Max = Vector3{ topRight, vCenter, bottomRight };
	Vector3 Min = Vector3{ topLeft, vCenter, bottomLeft };
	Vector3 Center = Max + Min;

	// 범위
	pNode->m_hBox.fExtent[0] = Max.x - Min.x;
	pNode->m_hBox.fExtent[1] = Max.y - Min.y;
	pNode->m_hBox.fExtent[2] = Max.z - Min.z;

	pNode->m_CornerList[TOP_LEFT] = Vector3{ Min.x, vCenter, Max.z };
	pNode->m_CornerList[TOP_RIGHT] = Max;
	pNode->m_CornerList[BOTTOM_LEFT] = Min;
	pNode->m_CornerList[BOTTOM_RIGHT] = Vector3{ Max.x, vCenter, Min.z };

	pNode->m_hBox.vMax = Max;
	pNode->m_hBox.vMin = Min;
	pNode->m_hBox.vCenter = Center;

	
	return pNode;
}

HQuadTree::HQuadTree(void)
{
	m_DivideSize = 4;
}

HQuadTree::~HQuadTree(void)
{
}
