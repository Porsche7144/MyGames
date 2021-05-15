//#include "HModelObj.h"
//
//void HSubMesh::SetUniqueBuffer(HTriangle& tri)
//{
//	for (int iVertex = 0; iVertex < 3; iVertex++)
//	{
//		bool bAdd = true;
//		int iPosReturn = -1;
//		for (int iIndex = 0; iIndex < m_VertexList.size(); iIndex++)
//		{
//			if (m_VertexList[iIndex] == tri.vVertex[iVertex])
//			{
//				m_IndexArray.push_back(iIndex);
//				bAdd = false;
//				break;
//			}
//		}
//		if (bAdd)
//		{
//			m_VertexList.push_back(tri.vVertex[iVertex]);
//			m_IWVertexList.push_back(tri.vIWVertex[iVertex]);
//			m_IndexArray.push_back(m_VertexList.size() - 1);
//		}
//	}
//	m_iNumFace++;
//}
//
//bool HModelObj::CreateInputLayOut()
//{
//	HRESULT hr = S_OK;
//
//	// LayOut
//	const D3D11_INPUT_ELEMENT_DESC layout[] =
//	{
//		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
//
//		{ "INDEX",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "WEIGHT",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
//
//	};
//
//	UINT iNumElement = sizeof(layout) / sizeof(layout[0]);
//	hr = g_pd3dDevice->CreateInputLayout(layout, iNumElement, pVSObj->GetBufferPointer(),
//		pVSObj->GetBufferSize(), &m_pInputLayout);
//	if (FAILED(hr))
//	{
//		return false;
//	}
//
//	return true;
//}