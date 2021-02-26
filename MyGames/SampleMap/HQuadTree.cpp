#include "HQuadTree.h"

bool HQuadTree::Build(float fWidth, float fHeight)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_pRootNode = CreateNode(NULL, -fWidth / 2, fWidth / 2, -fHeight / 2, fHeight / 2);

	if (BuildTree(m_pRootNode))
	{
		return true;
	}

	return false;
}

bool HQuadTree::BuildTree(HNode* pNode)
{
	// 분할 가능한지
	if (SubDivide(pNode))
	{
		for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
		{
/*		if (m_bUsedIndexList)
		{
			DWORD dwIndex =
				pNode->m_ChildList[iNode]->m_dwPositionIndex[1] * pow(2.0f, (float)pNode->m_ChildList[iNode]->m_dwDepth) + pNode->m_ChildList[iNode]->m_dwPositionIndex[0];
			DWORD dwValue = pNode->m_ChildList[iNode]->m_dwDepth;
			m_LevelList[dwValue][dwIndex] =
				pNode->m_ChildList[iNode];
		}*/
			BuildTree(pNode->m_ChildList[iNode]);
		}
	}
	
	return true;
}

bool HQuadTree::SubDivide(HNode* pNode)
{
	// 4등분 가능한지
	if (pNode == NULL)
	{
		return false;
	}

	// 최대 깊이 한도 초과시 강제 리프노트 지정
	if ((DWORD)m_iMaxDepthLimit <= pNode->m_dwDepth)
	{
		pNode->m_isLeaf = TRUE;
		return false;
	}
	
	// 현재 노드의 실제 크기 계산
	float fWidthSplit = (pNode->m_CornerList[TR].x - pNode->m_CornerList[TL].x) / 2;
	float fHeightSplit = (pNode->m_CornerList[TL].z - pNode->m_CornerList[BL].z) / 2;

	// 자식 노드가 지정된 분할 크기보다 작다면, 더이상 분할하지 않는다.
	if (fWidthSplit < m_fMinDivideSize || fHeightSplit < m_fMinDivideSize)
	{
		pNode->m_isLeaf = true;
		return false;
	}

	pNode->m_ChildList.push_back(CreateNode(pNode, pNode->m_CornerList[TL].x,
		pNode->m_CornerList[TL].x + fWidthSplit,
		pNode->m_CornerList[TL].z - fHeightSplit,
		pNode->m_CornerList[TL].z));

	pNode->m_ChildList.push_back(CreateNode(pNode, pNode->m_CornerList[TL].x + fWidthSplit,
		pNode->m_CornerList[TR].x,
		pNode->m_CornerList[TL].z - fHeightSplit,
		pNode->m_CornerList[TL].z));

	pNode->m_ChildList.push_back(CreateNode(pNode, pNode->m_CornerList[TL].x,
		pNode->m_CornerList[TL].x + fWidthSplit,
		pNode->m_CornerList[BL].z,
		pNode->m_CornerList[TL].z - fHeightSplit));

	pNode->m_ChildList.push_back(CreateNode(pNode, pNode->m_CornerList[TL].x + fWidthSplit,
		pNode->m_CornerList[TR].x,
		pNode->m_CornerList[BR].z,
		pNode->m_CornerList[TL].z - fHeightSplit));

	return true;
}

HNode* HQuadTree::CreateNode(HNode* pParentNode, float fTopLeft, float fTopRight, float fBottomLeft, float fBottomRight)
{
	HNode* pNode = 0;
	SAFE_NEW(pNode, HNode);
	assert(pNode);

	pNode->m_ChildList.reserve(4);
	pNode->m_CornerList.reserve(4);

	pNode->m_hBox.vMin = Vector3(fTopLeft, 0.0f, fBottomLeft);
	pNode->m_hBox.vMax = Vector3(fTopRight, 0.0f, fBottomRight);
	pNode->m_hBox.vCenter = (pNode->m_hBox.vMax + pNode->m_hBox.vMin);
	pNode->m_hBox.vCenter.x /= 2.0f;
	pNode->m_hBox.vCenter.y /= 2.0f;
	pNode->m_hBox.vCenter.z /= 2.0f;
	pNode->m_hBox.fExtent[0] = pNode->m_hBox.vMax.x - pNode->m_hBox.vCenter.x;
	pNode->m_hBox.fExtent[1] = pNode->m_hBox.vMax.y - pNode->m_hBox.vCenter.y;
	pNode->m_hBox.fExtent[2] = pNode->m_hBox.vMax.z - pNode->m_hBox.vCenter.z;

	pNode->m_CornerList.push_back(Vector3(pNode->m_hBox.vMin.x, 0.0f, pNode->m_hBox.vMax.z));
	pNode->m_CornerList.push_back(pNode->m_hBox.vMax);
	pNode->m_CornerList.push_back(pNode->m_hBox.vMin);
	pNode->m_CornerList.push_back(Vector3(pNode->m_hBox.vMax.x, 0.0f, pNode->m_hBox.vMin.z));

	if (pParentNode)
	{
		pNode->m_dwDepth = pParentNode->m_dwDepth + 1;
		if ((DWORD)m_iMaxDepth < pNode->m_dwDepth)
		{
			m_iMaxDepth = pNode->m_dwDepth;
		}
	}

	return pNode;

}

int HQuadTree::AddObject(HBaseObj* pObj)
{
	if (CheckRect(m_pRootNode, pObj))
	{
		HNode* pNode = FindNode(m_pRootNode, pObj);
		if (pNode)
		{
			pNode->m_ObjectList.push_back(pObj);
		}
		return 1;
	}

	return 0;
}

int HQuadTree::CheckRect(HNode* pNode, HBaseObj* pObj)
{
	if (pNode->m_hBox.vMin.x <= pObj->m_hBox.vMin.x && pNode->m_hBox.vMax.x >= pObj->m_hBox.vMax.x)
	{
		if (pNode->m_hBox.vMin.z <= pObj->m_hBox.vMin.z && pNode->m_hBox.vMax.z >= pObj->m_hBox.vMax.z)
		{
			return 1;
		}
	}
	return 0;
}

HNode* HQuadTree::FindNode(HNode* pNode, HBaseObj* pObj)
{
	assert(pNode);
	do
	{
		for (DWORD dwChild = 0; dwChild < pNode->m_ChildList.size(); dwChild++)
		{
			if (pNode->m_ChildList[dwChild] && CheckRect(pNode->m_ChildList[dwChild], pObj))
			{
				m_pQuadTreeQueue.push(pNode->m_ChildList[dwChild]);
			}
		}

		if (m_pQuadTreeQueue.empty())
		{
			break;
		}

		pNode = m_pQuadTreeQueue.front();
		m_pQuadTreeQueue.pop();

	} while (pNode);

	return pNode;
}

bool HQuadTree::PreFrame()
{
	m_DrawNodeList.clear();
	m_DrawObjList.clear();

	return true;
}

bool HQuadTree::Frame()
{
	if (!PreFrame()) return true;

	DrawFindNode(m_pRootNode);
	return true;
}

bool HQuadTree::Release()
{
	SAFE_DEL(m_pRootNode);
	return true;
}

void HQuadTree::VisibleNode(HNode* pNode)
{
	////assert(m_pCamera);
	//if (pNode->m_dwDepth < (DWORD)m_iRenderDepth) return;

	//if (m_pCamera->CheckOBBInPlane(&pNode->m_tBox))
	//{
	//	VisibleObject(pNode);
	//	for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
	//	{
	//		VisibleNode(pNode->m_ChildList[iNode]);
	//	}
	//}
}

void HQuadTree::VisibleObject(HNode* pNode)
{
	for (int iObj = 0; iObj < pNode->m_ObjectList.size(); iObj++)
	{
		/*if (m_pCamera->CheckOBBInPlane(&pNode->m_ObjectList[iObj]->m_tBox))
		{
			m_DrawObjList.push_back(pNode->m_ObjectList[iObj]);
		}*/
	}
}

void HQuadTree::DrawFindNode(HNode* pNode)
{
	//assert(pNode);

	//T_POSITION t_Pos = m_pCamera->CheckPoitionOBBInPlane(&pNode->m_tBox);
	//if (pNode->m_dwDepth < (DWORD)m_iRenderDepth ||
	//	t_Pos == P_BACK)
	//{
	//	return;
	//}
	//GetLodSubIndex(pNode);

	//// 리프노드 일 경우는 완전히 제외되지 않았다면(걸쳐 있거나 완전 포함)추가한다.
	//if (pNode->m_isLeaf &&  t_Pos != P_BACK)
	//{
	//	m_DrawNodeList.push_back(pNode);
	//	VisibleObject(pNode);
	//	return;
	//}
	//// 완전히 포함되어 있으면 자식을 탐색하지 않고 노드를 추가한다.
	//if (t_Pos == P_FRONT)
	//{
	//	m_DrawNodeList.push_back(pNode);
	//	VisibleNode(pNode);
	//	return;
	//}

	//// 걸쳐져 있는 노드에 포함된 오브젝트 체크
	//if (t_Pos == P_SPANNING)
	//{
	//	VisibleObject(pNode);
	//}

	//for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
	//{
	//	DrawFindNode(pNode->m_ChildList[iNode]);
	//}
}

void HQuadTree::SetRenderDepth(DWORD dwDepth)
{
	/*if ((DWORD)m_iMaxDepth <= dwDepth) m_iRenderDepth = -1;
	else						m_iRenderDepth = dwDepth;*/
}

void HQuadTree::FindNeighborNode(HNode* pNode)
{
	//pNode->m_NeighborList.resize(4);

	//DWORD dwNumPatchCount = (DWORD)pow(2.0f, (float)pNode->m_dwDepth);

	//DWORD dwNeighborCol, dwNeighborRow;
	//if (pNode->m_dwPositionIndex[1] > 0)  // 상
	//{
	//	dwNeighborCol = pNode->m_dwPositionIndex[0];
	//	dwNeighborRow = (pNode->m_dwPositionIndex[1] - 1) * dwNumPatchCount;
	//	pNode->m_NeighborList[0] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	//}
	//if (pNode->m_dwPositionIndex[1] < dwNumPatchCount - 1) // 하
	//{
	//	dwNeighborCol = pNode->m_dwPositionIndex[0];
	//	dwNeighborRow = (pNode->m_dwPositionIndex[1] + 1) * dwNumPatchCount;
	//	pNode->m_NeighborList[1] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	//}
	//if (pNode->m_dwPositionIndex[0] > 0) // 좌
	//{
	//	dwNeighborCol = pNode->m_dwPositionIndex[0] - 1;
	//	dwNeighborRow = pNode->m_dwPositionIndex[1] * dwNumPatchCount;
	//	pNode->m_NeighborList[2] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	//}
	//if (pNode->m_dwPositionIndex[0] < dwNumPatchCount - 1) // 우
	//{
	//	dwNeighborCol = pNode->m_dwPositionIndex[0] + 1;
	//	dwNeighborRow = pNode->m_dwPositionIndex[1] * dwNumPatchCount;
	//	pNode->m_NeighborList[3] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	//}
}

void HQuadTree::SetNeighborNode(HNode* pNode)
{
	FindNeighborNode(pNode);
	for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
	{
		SetNeighborNode(pNode->m_ChildList[iNode]);
	}
}

DWORD HQuadTree::CheckSize(DWORD dwSize)
{
	float fCount = 0;
	DWORD dwChecker = dwSize / 2;
	if (dwChecker <= 0) return 0;

	while (dwChecker != 0)
	{
		dwChecker >>= 1;
		fCount += 1.0f;
	}

	float fMinimum = pow(2.0f, fCount - 1.0f);
	float fMaximum = pow(2.0f, fCount);

	// fMin = abs( fMin - P0 ) - abs( P1 - fMin );
	// fMax = abs( fMax - P0 ) - abs( P1 - fMax );
	// return min( fMin, fMax );

	float fMin = fabs(fMinimum - fabs(((float)dwSize - fMinimum)));
	float fMax = fabs(fMaximum - fabs(((float)dwSize - fMaximum)));

	// 비교값이 같다면 작은 분할크기를 반환한다. 예) fMin=1 : fMax=1 => fMinimum=2 : fMaximum=4
	if (fMin <= fMax)
	{
		return (DWORD)fMinimum;
	}
	return (DWORD)fMaximum;
}

HQuadTree::HQuadTree(void)
{
	m_iRenderDepth = 0;
	m_iMaxDepth = 0;
	m_iMaxDepthLimit = 4;
	m_fMinDivideSize = 2;
	m_bUsedIndexList = false;
	m_pd3dDevice = 0;
	m_pRootNode = 0;
}

HQuadTree::~HQuadTree(void)
{
	Release();
}
