#include "HMap.h"

float HMap::GetHeightMap(int row, int col)
{
	return m_VertexList[row * m_iNumRows + col].p.y;
}

float HMap::Lerp(float fStart, float fEnd, float fTangent)
{
	return fStart - (fStart * fTangent) + (fEnd * fTangent);
}

float HMap::GetHeightMap(float fPosX, float fPosZ)
{

	// fPosX / fPosZ 위체애 해당하는 높이맵 셀을 찾는다.
	// m_iNumCols 와 m_iNumRows는 가로/세로의 실제 크기값이다.
	float fCellX = (float)(m_iNumCellCols * m_fCellDistance / 2.0f + fPosX);
	float fCellZ = (float)(m_iNumCellRows * m_fCellDistance / 2.0f - fPosZ);

	// 셀의 크기로 나누어 0~1 크기의 값으로 바꿔서 높이맵 배열에 접근
	fCellX /= float(m_fCellDistance);
	fCellZ /= float(m_fCellDistance);

	// fCellX, fCellZ 값보다 작거나, 같은 최대 정수(소수부분은 잘라냄.)
	// floorf 소수점 자리 이하 버리는 함수
	float fVertexCol = ::floorf(fCellX);
	float fVertexRow = ::floorf(fCellZ);

	// 높이맵 범위를 벗어나면 강제 초기화.
	if (fVertexCol < 0.0f)	fVertexCol = 0.0f;
	if (fVertexRow < 0.0f) fVertexRow = 0.0f;
	if ((float)(m_iNumCols - 2) < fVertexCol) fVertexCol = (float)(m_iNumCols - 2);
	if (float(m_iNumRows - 2) < fVertexRow) fVertexRow = (float)(m_iNumRows - 2);

	// 계산된 셀의 플랜을 구성하는 4개 정점의 높이값을 찾는다. 
	//  A   B
	//  *---*
	//  | / |
	//  *---*  
	//  C   D
	float A = GetHeightMap((int)fVertexRow, (int)fVertexCol);
	float B = GetHeightMap((int)fVertexRow, (int)fVertexCol +1);
	float C = GetHeightMap((int)fVertexRow +1, (int)fVertexCol);
	float D = GetHeightMap((int)fVertexRow +1, (int)fVertexCol +1);

	// A정점 위치에서 떨어진 값(변위값)을 계산. 0 ~ 1.0f
	float fDeltaX = fCellX - fVertexCol;
	float fDeltaZ = fCellZ - fVertexRow;

	// 보간작업을 위한 기준 페이스를 찾는다.
	float fHeight = 0.0f;
	// 윗페이스 기준으로 보간.
	// fDeltaZ + fDeltaX < 1.0f
	if (fDeltaZ < (1.0f - fDeltaX)) // ABC
	{
		float uy = B - A; // A -> B
		float vy = C - A; // A -> C
		  // 두 정점의 높이값의 차이를 비교하여 델타X의 값에 따라 보간값을 찾는다.	
		fHeight = A + Lerp(0.0f, uy, fDeltaX) + Lerp(0.0f, vy, fDeltaZ);
	}
	// 아래페이스 기준으로 보간.
	else
	{
		float uy = C - D; // D -> C
		float vy = B - D; // D -> B
		  // 두 정점의 높이값의 차이를 비교하여 델타Z의 값에 따라 보간값을 찾는다.
		fHeight = D + Lerp(0.0f, uy, 1.0f - fDeltaX) + Lerp(0.0f, vy, 1.0f - fDeltaZ);
	}

	return fHeight;
}

float HMap::GetHeight(UINT index)
{
	return 0.0f;
}

bool HMap::CreateVertexData()
{
	// 정점 가로/세로 = 2N승+1

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

			// 얻어온 높이만큼 할당.
			m_VertexList[iIndex].p.y = GetHeight(iIndex);
			m_VertexList[iIndex].p.z = (iRow - fHalfRows) * m_fCellDistance*-1.0f;
			m_VertexList[iIndex].t.x = iCol * fOffsetU * 1;
			m_VertexList[iIndex].t.y = iRow * fOffsetV * 1;

			//m_VertexList[iIndex].n = GetNormalOfVertex(iIndex);
			m_VertexList[iIndex].c = { 1,1,1,1 };
		}

	}

	return true;
}

Vector3 HMap::GetNormalOfVertex(UINT index)
{
	return Vector3(0.0f, 0.1f, 0.0f);
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

	m_iNumFaces = m_IndexList.size() / 3;

	// 노말값 받아오기
	GetVertexNormal();

	return true;
}

Vector3 HMap::ComputeFaceNormal(DWORD i0, DWORD i1, DWORD i2)
{
	Vector3 normal;
	Vector3 v0 = m_VertexList[i1].p - m_VertexList[i0].p;
	Vector3 v1 = m_VertexList[i2].p - m_VertexList[i0].p;
	normal = v0.Cross(v1);
	normal.Normalize();

	return normal;
}

void HMap::GetVertexNormal()
{
	InitFaceNormal();
	GenNormalLookUpTable();
	CalcPerVertexNormalsFastLookUp();
}

void HMap::InitFaceNormal()
{
	m_FaceNormals.resize(m_iNumFaces);
	for (int i = 0; i < m_iNumFaces; i++)
	{
		m_FaceNormals[i] = Vector3::Zero;
	}
}

void HMap::GenNormalLookUpTable()
{
	m_LookUpTable.resize(m_iNumVertices);

	// 페이스에
	for (int iFace = 0; iFace < m_iNumFaces; iFace++)
	{
		// 페이스 각 정점에
		for (int j = 0; j < 3; j++)
		{
			// 노말 수
			for (int k = 0; k < 6; k++)
			{
				int id = m_IndexList[iFace * 3 + j];
				if (m_LookUpTable[id].Index[k] == -1)
				{
					m_LookUpTable[id].Index[k] = iFace;
					break;
				}
			}
		}
	}
}

void HMap::CalcFaceNormals()
{
	int index = 0;
	for (int i = 0; i < m_iNumFaces * 3; i += 3)
	{
		m_FaceNormals[index++] = ComputeFaceNormal(
			m_IndexList[i], 
			m_IndexList[i + 1], 
			m_IndexList[i + 2]);
	}
}

void HMap::CalcPerVertexNormalsFastLookUp()
{
	CalcFaceNormals();

	for (int i = 0; i < m_iNumVertices; i++)
	{
		Vector3 avgNormal = { 0,0,0 };

		for (int j = 0; j < 6; j++)
		{
			int index = m_LookUpTable[i].Index[j];
			if (index != -1)
			{
				avgNormal += m_FaceNormals[index];
			}
			else
			{
				break;
			}
		}
		m_VertexList[i].n = avgNormal;
		m_VertexList[i].n.Normalize();
	}
}

bool HMap::Frame()
{
	return true;
}

bool HMap::PostRender(ID3D11DeviceContext * pd3dContext)
{
	pd3dContext->DrawIndexed(m_iNumFaces*3, 0, 0);

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


	Create(pDevice, desc.szVS, desc.szPS, desc.szTextFile);

	return true;
}

HMap::HMap()
{
}

HMap::~HMap()
{
}
