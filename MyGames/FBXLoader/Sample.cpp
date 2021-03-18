#include "Sample.h"
#include "HTextureManager.h"

LRESULT	 Sample::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pMainCamera == nullptr) return -1;
	m_pMainCamera->WndProc(hWnd, message, wParam, lParam);

	return -1;
}

bool Sample::Init()
{
	m_Camera.CreateViewMatrix({ 0,50,-50 }, { 0,0,0 });

	m_FbxObj.Load("../../Image/FBX_Image/ship.fbx");

	for (auto data : m_FbxObj.m_hMeshMap)
	{
		HModelObject* pObj = (HModelObject*)data.second;
		if (pObj->m_TriangleList.size() <= 0 && pObj->m_SubMesh.size() <= 0) continue;

		// 서브 메테리얼이 없다면
		if (pObj->m_SubMesh.size() == 0)
		{
			pObj->m_VertexList.resize(pObj->m_TriangleList.size() * 3);
			for (int iFace = 0; iFace < pObj->m_TriangleList.size(); iFace++)
			{
				int iIndex = iFace * 3;
				pObj->m_VertexList[iIndex] = pObj->m_TriangleList[iFace].vVertex[0];
				pObj->m_VertexList[iIndex + 1] = pObj->m_TriangleList[iFace].vVertex[1];
				pObj->m_VertexList[iIndex + 2] = pObj->m_TriangleList[iFace].vVertex[2];
			}

			T_STR LoadTexName = L"../../Image/";
			LoadTexName += pObj->FbxMaterialList[0];

			if (!pObj->Create(g_pd3dDevice, L"VS.txt", L"PS.txt", LoadTexName))
			{
				return false;
			}
		}
		else
		{
			HSubMesh* pSubMesh;
			wstring LoadTex;
			for (int iSub = 0; iSub < pObj->m_SubMesh.size(); iSub++)
			{
				pSubMesh = &pObj->m_SubMesh[iSub];
				pSubMesh->m_VertexList.resize(pSubMesh->m_TriangleList.size() * 3);

				for (int iFace = 0; iFace < pSubMesh->m_TriangleList.size(); iFace++)
				{
					int iIndex = iFace * 3;
					pObj->m_SubMesh[iSub].m_VertexList[iIndex] = pObj->m_SubMesh[iSub].m_TriangleList[iFace].vVertex[0];
					pObj->m_SubMesh[iSub].m_VertexList[iIndex + 1] = pObj->m_SubMesh[iSub].m_TriangleList[iFace].vVertex[1];
					pObj->m_SubMesh[iSub].m_VertexList[iIndex + 2] = pObj->m_SubMesh[iSub].m_TriangleList[iFace].vVertex[2];
				}

				ID3D11Buffer* pVertexBuffer = CreateVertexBuffer(g_pd3dDevice, &pSubMesh->m_VertexList.at(0),
																pSubMesh->iCount, sizeof(PNCT_VERTEX));
				
				pSubMesh->m_pVertexBuffer.Attach(pVertexBuffer);
				LoadTex = L"../../Image/FBX_Image/";
				LoadTex += pObj->FbxMaterialList[iSub].c_str();
				pSubMesh->m_pTexture = g_TextureMgr.Load(g_pd3dDevice, LoadTex.c_str());				
			}

			if (!pObj->Create(g_pd3dDevice, L"VS.txt", L"PS.txt", LoadTex))
			{
				return false;
			}
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
	//for (auto data : m_FbxObj.m_hMeshMap)
	//{
	//	HObject* pObj = (HObject*)data.second;
	//	if (pObj->m_TriangleList.empty()) continue;

	//	pObj->SetMatrix(&pObj->m_matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
	//	pObj->Render(g_pImmediateContext);

	//}

	for (auto data : m_FbxObj.m_hMeshMap)
	{
		HModelObject* pObj = (HModelObject*)data.second;
		for (int iSub = 0; iSub < pObj->m_SubMesh.size(); iSub++)
		{
			if (pObj->m_SubMesh[iSub].m_TriangleList.empty()) continue;

			pObj->SetMatrix(&pObj->m_matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
			pObj->Render(g_pImmediateContext);
		}

		

	}

	return true;
};

bool Sample::Release()
{
	return true;
};
