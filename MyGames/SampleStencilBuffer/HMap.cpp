#include "HMap.h"

bool HMap::CreateVertexData()
{
	m_VertexList.resize(m_iNumVertices);
	float fHalfCols = (m_iNumCols - 1) / 2.0f;
	float fHalfRows = (m_iNumRows - 1) / 2.0f;
	float fOffsetU = 1.0f / (m_iNumCols - 1);
	float fOffsetV = 1.0f / (m_iNumRows - 1);

	for (int iRow = 0; iRow < m_iNumRows; iRow++)
	{

		for (int iCol = 0; iCol < m_iNumCols; iCol++)
		{
			int iIndex = iRow * m_iNumCols + iCol;
			m_VertexList[iIndex].p.x = (iCol-fHalfCols) * m_fCellDistance;
			m_VertexList[iIndex].p.y = 0.0f;
			m_VertexList[iIndex].p.z = (iRow - fHalfRows) * m_fCellDistance*-1.0f;
			m_VertexList[iIndex].t.x = iCol * fOffsetU;
			m_VertexList[iIndex].t.y = iRow * fOffsetV;

			m_VertexList[iIndex].n = { 0,1,0 };
			m_VertexList[iIndex].c = { 1,1,1,1 };

		}

	}

	return true;
}

bool HMap::CreateIndexData()
{
	m_IndexList.resize(m_iNumFaces * 3);
	int iIndex = 0;
	for (int iRow = 0; iRow < m_iNumCellRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCellCols; iCol++)
		{
			int iNextRow = iRow + 1;
			int iNextCol = iCol + 1;
			m_IndexList[iIndex + 0] = iRow * m_iNumCols + iCol;
			m_IndexList[iIndex + 1] = iRow * m_iNumCols + iNextCol;
			m_IndexList[iIndex + 2] = iNextRow * m_iNumCols + iCol;
			m_IndexList[iIndex + 3] = m_IndexList[iIndex + 2];
			m_IndexList[iIndex + 4] = m_IndexList[iIndex + 1];
			m_IndexList[iIndex + 5] = iNextRow * m_iNumCols + iNextCol;

			iIndex += 6;
		}
	}

	return true;
}

bool HMap::CreateMap(ID3D11Device* pDevice, HMapDesc desc)
{
	m_MapDesc = desc;
	m_iNumRows = desc.iNumRows;
	m_iNumCols = desc.iNumCols;
	m_iNumCellCols = m_iNumCols - 1;
	m_iNumCellRows = m_iNumRows - 1;
	m_iNumVertices = m_iNumCols * m_iNumRows;
	m_iNumFaces = m_iNumCellCols * m_iNumCellRows * 2;
	m_fCellDistance = desc.fCellDistance;

	std::vector<float> m_fHeightLIst;
	m_fHeightLIst.resize(m_iNumVertices);

	Create(pDevice, desc.szVS, desc.szPS, desc.szTextFile);

	return true;
}

HMap::HMap()
{
}

HMap::~HMap()
{
}
