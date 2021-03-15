#include "Sample.h"

bool Sample::Init()
{
	m_FbxObj.Load("../../Image/FBX_Image/Card.fbx");

	for (auto data : m_FbxObj.m_hMeshMap)
	{
		HObject* pObj = (HObject*)data.second;
		if (pObj->m_TriangleList.empty()) continue;

		pObj->m_VertexList.resize(pObj->m_TriangleList.size() * 3);
		for (int iFace = 0; iFace < pObj->m_TriangleList.size(); iFace++)
		{
			int iIndex = iFace * 3;
			pObj->m_VertexList[iIndex] = pObj->m_TriangleList[iFace].vVertex[0];
			pObj->m_VertexList[iIndex +1] = pObj->m_TriangleList[iFace].vVertex[1];
			pObj->m_VertexList[iIndex +2] = pObj->m_TriangleList[iFace].vVertex[2];
		}

		if (!pObj->Create(g_pd3dDevice, L"VS.txt", L"PS.txt", L"../../Image/tileA.jpg"))
		{
			return false;
		}
	}

	return true;
};

bool Sample::Frame()
{
	return true;
};

bool Sample::Render()
{
	for (auto data : m_FbxObj.m_hMeshMap)
	{
		HObject* pObj = (HObject*)data.second;
		if (pObj->m_TriangleList.empty()) continue;

		pObj->SetMatrix(&pObj->m_matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
		pObj->Render(g_pImmediateContext);
	}

	return true;
};

bool Sample::Release()
{
	return true;
};
