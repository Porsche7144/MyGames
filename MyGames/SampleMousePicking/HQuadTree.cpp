#include "HQuadTree.h"

void HQuadTree::CreateIndexNode(HNode* pNode)
{
	
	vector<DWORD> Indexlist;
	DWORD dwTL = pNode->m_CornerIndex[0];
	DWORD dwTR = pNode->m_CornerIndex[1];
	DWORD dwBL = pNode->m_CornerIndex[2];
	DWORD dwBR = pNode->m_CornerIndex[3];

	DWORD dwSize = (dwTR - dwTL)*(dwTR - dwTL) * 2 * 3;
	pNode->m_IndexList.resize(dwSize);
	int dwCurentIndex = 0;
	DWORD m_dwWidth = m_pMap->m_iNumCols;
	DWORD dwStartRow = dwTL / m_dwWidth;
	DWORD dwEndRow = dwBL / m_dwWidth;

	DWORD dwStartCol = dwTL % m_dwWidth;
	DWORD dwEndCol = dwTR % m_dwWidth;
	//  0      
	for (DWORD dwRow = dwStartRow; dwRow < dwEndRow; dwRow++)
	{
		for (DWORD dwCol = dwStartCol; dwCol < dwEndCol; dwCol++)
		{
			//0	1    4   
			//2	   3 5
			DWORD dwNextRow = dwRow + 1;
			DWORD dwNextCol = dwCol + 1;
			pNode->m_IndexList[dwCurentIndex++] = dwRow * m_dwWidth + dwCol;
			pNode->m_IndexList[dwCurentIndex++] = dwRow * m_dwWidth + dwNextCol;
			pNode->m_IndexList[dwCurentIndex++] = dwNextRow * m_dwWidth + dwCol;
			pNode->m_IndexList[dwCurentIndex++] = dwNextRow * m_dwWidth + dwCol;
			pNode->m_IndexList[dwCurentIndex++] = dwRow * m_dwWidth + dwNextCol;
			pNode->m_IndexList[dwCurentIndex++] = dwNextRow * m_dwWidth + dwNextCol;
		}
	}

	pNode->m_pIndexBuffer = CreateIndexBuffer(g_pd3dDevice, &pNode->m_IndexList.at(0),
		pNode->m_IndexList.size(), sizeof(DWORD));

}

void HQuadTree::CreateVertexNode(HNode* pNode)
{
	vector<DWORD> VertexList;
	DWORD dwTL = pNode->m_CornerIndex[0];
	DWORD dwTR = pNode->m_CornerIndex[1];
	DWORD dwBL = pNode->m_CornerIndex[2];
	DWORD dwBR = pNode->m_CornerIndex[3];

	DWORD dwSize = ((dwTR - dwTL) +1)*((dwTR - dwTL) +1);
	pNode->m_VertexList.resize(dwSize);
	int dwCurentIndex = 0;
	DWORD m_dwWidth = m_pMap->m_iNumCols;
	DWORD dwStartRow = dwTL / m_dwWidth;
	DWORD dwEndRow = dwBL / m_dwWidth;

	DWORD dwStartCol = dwTL % m_dwWidth;
	DWORD dwEndCol = dwTR % m_dwWidth;

	for (DWORD dwRow = dwStartRow; dwRow < dwEndRow; dwRow++)
	{
		for (DWORD dwCol = dwStartCol; dwCol < dwEndCol; dwCol++)
		{
			pNode->m_VertexList[dwCurentIndex++] = m_pMap->m_VertexList[dwRow * m_dwWidth + dwCol];
		}
	}

	pNode->m_pVertexBuffer = CreateVertexBuffer(g_pd3dDevice, &pNode->m_VertexList.at(0),
		pNode->m_VertexList.size(), sizeof(DWORD));
}

 // 0 31 63

bool HQuadTree::Build(HMap* map)
{
	
	m_pMap = map;
	float Width = map->m_iNumCols -1;
	float Height = map->m_iNumRows -1;
	float Vertices = map->m_iNumVertices - 1;

	m_ltPos->TopLeft = Width;
	m_ltPos->TopRight = 0;
	m_ltPos->BottomLeft = map->m_iNumVertices - map->m_iNumCols;
	m_ltPos->BottomeRight = Vertices - 1;

	m_pParentNode = CreateTreeNode(m_ltPos->TopRight, m_ltPos->TopLeft,
								m_ltPos->BottomLeft, m_ltPos->BottomeRight);

	m_pParentNode->m_dwDepth = 0;

	if (NodeDivide(m_pParentNode))
	{
		return true;
	}

	return false;
}

bool HQuadTree::SetChildTree(HNode* pNode)
{
	UINT iDepth = pNode->m_dwDepth + 1;
	pNode->m_isLeaf = FALSE;

	if ((pNode->m_CornerIndex[1] - pNode->m_CornerIndex[0] <= 1))
	{
		pNode->m_isLeaf = TRUE;
		return false;
	}
	if (iDepth < 4)
	{
		pNode->m_isLeaf = FALSE;
		return true;
	}
	pNode->m_isLeaf = TRUE;
	m_leafList.push_back(pNode);

	return false;
}

bool HQuadTree::Render(ID3D11DeviceContext * pContext)
{
	DrawNode(pContext, m_pParentNode);
	return true;
}

bool HQuadTree::Release()
{
	delete m_pParentNode;
	return true;
}

bool HQuadTree::NodeDivide(HNode* pNode)
{

	if (SetChildTree(pNode))
	{
		// 노드의 가운데
		float fHalfWidth = (pNode->m_CornerIndex[TOP_RIGHT] - pNode->m_CornerIndex[TOP_LEFT]) / 2;
		float fHalfHeight = (pNode->m_CornerIndex[BOTTOM_RIGHT] - pNode->m_CornerIndex[BOTTOM_LEFT]) / 2;

		// 0  1  e0  3  4
		// 5  6  7  8   9 
		// e1 11 e2 13 e3
		// 15 16 17 18 19 
		// 20 21 e4 23 24

		DWORD e0 = pNode->m_CornerIndex[TOP_LEFT] + fHalfWidth;
		DWORD e1 = pNode->m_CornerIndex[TOP_LEFT] + (m_pMap->m_iNumCols * fHalfWidth);
		DWORD e2 = e0 + (m_pMap->m_iNumCols * fHalfWidth);
		DWORD e3 = pNode->m_CornerIndex[TOP_RIGHT] + (m_pMap->m_iNumCols * fHalfWidth);
		DWORD e4 = pNode->m_CornerIndex[BOTTOM_RIGHT] - fHalfHeight;


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
		//m_ltPos[0].TopLeft = pNode->m_CornerList[TOP_LEFT].x;
		//m_ltPos[0].TopRight = pNode->m_CornerList[TOP_LEFT].x + fHalfWidth;
		//m_ltPos[0].BottomLeft = pNode->m_CornerList[TOP_LEFT].z - fHalfHeight;
		//m_ltPos[0].BottomeRight = pNode->m_CornerList[TOP_LEFT].z;


		////  ---------
		//// |
		//// |
		//m_ltPos[1].TopLeft = pNode->m_CornerList[TOP_LEFT].x + fHalfWidth;
		//m_ltPos[1].TopRight = pNode->m_CornerList[TOP_RIGHT].x;
		//m_ltPos[1].BottomLeft = pNode->m_CornerList[TOP_LEFT].z - fHalfHeight;
		//m_ltPos[1].BottomeRight = pNode->m_CornerList[TOP_LEFT].z;

		////			|
		////			|
		////  --------
		//m_ltPos[2].TopLeft = pNode->m_CornerList[TOP_LEFT].x;
		//m_ltPos[2].TopRight = pNode->m_CornerList[TOP_LEFT].x + fHalfWidth;
		//m_ltPos[2].BottomLeft = pNode->m_CornerList[BOTTOM_LEFT].z;
		//m_ltPos[2].BottomeRight = pNode->m_CornerList[TOP_LEFT].z - fHalfHeight;

		//// |
		//// |
		////  ---------
		//m_ltPos[3].TopLeft = pNode->m_CornerList[TOP_LEFT].x + fHalfWidth;
		//m_ltPos[3].TopRight = pNode->m_CornerList[TOP_RIGHT].x;
		//m_ltPos[3].BottomLeft = pNode->m_CornerList[BOTTOM_RIGHT].z;
		//m_ltPos[3].BottomeRight = pNode->m_CornerList[TOP_LEFT].z - fHalfHeight;


		pNode->m_Child[0] = CreateTreeNode(
			pNode->m_CornerIndex[TOP_LEFT], e0, e1, e2);

		pNode->m_Child[1] = CreateTreeNode(
			e0, pNode->m_CornerIndex[TOP_RIGHT], e2, e3);

		pNode->m_Child[2] = CreateTreeNode(
			e1, e2, pNode->m_CornerIndex[BOTTOM_LEFT], e4);

		pNode->m_Child[3] = CreateTreeNode(
			e2, e3, e4, pNode->m_CornerIndex[BOTTOM_RIGHT]);

		NodeDivide(pNode->m_Child[0]);
		NodeDivide(pNode->m_Child[1]);
		NodeDivide(pNode->m_Child[2]);
		NodeDivide(pNode->m_Child[3]);
	}

	return true;
}

HNode* HQuadTree::CreateTreeNode(float topLeft, float topRight, float bottomLeft, float bottomRight)
{
	HNode* pNode = new HNode;

	//float vCenter = 0.0f;
	//Vector3 Max = Vector3{ topRight, vCenter, bottomRight };
	//Vector3 Min = Vector3{ topLeft, vCenter, bottomLeft };
	//Vector3 Center = Max + Min;

	// 범위
	//pNode->m_hBox.fExtent[0] = Max.x - Min.x;
	//pNode->m_hBox.fExtent[1] = Max.y - Min.y;
	//pNode->m_hBox.fExtent[2] = Max.z - Min.z;

	pNode->m_CornerIndex[0] = topLeft;
	pNode->m_CornerIndex[1] = topRight;
	pNode->m_CornerIndex[2] = bottomLeft;
	pNode->m_CornerIndex[3] = bottomRight;

	//pNode->m_hBox.vMax = Max;
	//pNode->m_hBox.vMin = Min;
	//pNode->m_hBox.vCenter = Center;

	CreateIndexNode(pNode);

	return pNode;
}

void HQuadTree::DrawNode(ID3D11DeviceContext * pContext, HNode* pNode)
{
	if (pNode == nullptr)
	{
		return;
	}

	if (pNode->m_isLeaf == TRUE)
	{
		HDxObject* pObject = (HDxObject*)m_pMap;
		pObject->Update(pContext);
		m_pMap->PreRender(pContext);
		UINT iStride = sizeof(PNCT_VERTEX);
		UINT iOffset = 0;
		pContext->IASetVertexBuffers(0, 1, &m_pMap->m_pVertexBuffer, &iStride, &iOffset);
		pContext->IASetIndexBuffer(
			pNode->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		pContext->IASetInputLayout(m_pMap->m_pInputLayout);
		pContext->VSSetConstantBuffers(0, 1, &m_pMap->m_ConstantBuffer);
		pContext->PSSetConstantBuffers(0, 1, &m_pMap->m_ConstantBuffer);
		pContext->VSSetShader(m_pMap->m_pVertexShader, NULL, 0);
		pContext->PSSetShader(m_pMap->m_pPixelShader, NULL, 0);
		pContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_pMap->m_iTopology);
		if (m_pMap->m_pTexture != nullptr)
		{
			pContext->PSSetShaderResources(0, 1,
				&m_pMap->m_pTexture->m_pTextureSRV);
		}
		pContext->DrawIndexed(pNode->m_IndexList.size(), 0, 0);

		return;
	}

	for (int iNode = 0; iNode < m_leafList.size(); iNode++)
	{
		DrawNode(pContext, pNode->m_Child[iNode]);
		/*DrawNode(pContext, pNode->m_Child[1]);
		DrawNode(pContext, pNode->m_Child[2]);
		DrawNode(pContext, pNode->m_Child[3]);*/
	}

}

HQuadTree::HQuadTree(void)
{
	m_DivideSize = 4;
}

HQuadTree::~HQuadTree(void)
{
}
