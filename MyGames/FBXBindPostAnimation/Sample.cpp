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

	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_Camera.CreateProjectionMatrix(1, 5000, HBASIS_PI / 4.0f, fAspect);

	m_pFbxObj = make_shared<HFbxObj>();

if (m_pFbxObj->Load("../../Image/FBX_Image/man.fbx"))
{
	for (auto data : m_pFbxObj->m_hNodeList)
	{
		HModelObj* pObject = data;
		if (pObject->m_TriangleList.size() <= 0 &&
			pObject->m_SubMesh.size() <= 0)
		{
			continue;
		}
		if (pObject->m_SubMesh.size() == 0)
		{
			pObject->m_VertexList.resize(pObject->m_TriangleList.size() * 3);
			for (int iFace = 0; iFace < pObject->m_TriangleList.size(); iFace++)
			{
				int iIndex = iFace * 3;
				pObject->m_VertexList[iIndex + 0] =
					pObject->m_TriangleList[iFace].vVertex[0];
				pObject->m_VertexList[iIndex + 1] =
					pObject->m_TriangleList[iFace].vVertex[1];
				pObject->m_VertexList[iIndex + 2] =
					pObject->m_TriangleList[iFace].vVertex[2];
			}

			T_STR loadTexName = L"../../Image/FBX_Image/";
			if (pObject->FbxMaterialList.size())
			{
				loadTexName += pObject->FbxMaterialList[0];
			}
			else
			{
				loadTexName.clear();
			}
			if (!pObject->Create(g_pd3dDevice,
				L"vs.txt",
				L"ps.txt",
				loadTexName))
			{
				return false;
			}

		}
		else
		{
			for (int iSub = 0; iSub < pObject->m_SubMesh.size(); iSub++)
			{
				HSubMesh* pSub = &pObject->m_SubMesh[iSub];
				if (pSub->m_TriangleList.size() <= 0) continue;

				pSub->m_VertexList.resize(
					pSub->m_TriangleList.size() * 3);
				for (int iFace = 0; iFace < pSub->m_TriangleList.size(); iFace++)
				{
					int iIndex = iFace * 3;
					pSub->m_VertexList[iIndex + 0] =
						pSub->m_TriangleList[iFace].vVertex[0];
					pSub->m_VertexList[iIndex + 1] =
						pSub->m_TriangleList[iFace].vVertex[1];
					pSub->m_VertexList[iIndex + 2] =
						pSub->m_TriangleList[iFace].vVertex[2];
				}

				// vb
				ID3D11Buffer* vb =
					CreateVertexBuffer(g_pd3dDevice,
						&pSub->m_VertexList.at(0),
						pSub->m_VertexList.size(),
						sizeof(PNCT_VERTEX));
				pSub->m_pVertexBuffer.Attach(vb);
				wstring loadTex = L"../../Image/FBX_Image/";
				loadTex += pObject->FbxMaterialList[iSub].c_str();
				pSub->m_pTexture = g_TextureMgr.Load(g_pd3dDevice, loadTex.c_str());

			}

			if (!pObject->Create(g_pd3dDevice,
				L"vs.txt",
				L"ps.txt",
				L""))
			{
				return false;
			}
		}
	}
}


return true;
};

bool Sample::Frame()
{
	m_pFbxObj->m_fTick += g_fSecondPerFrame * m_pFbxObj->m_AnimScene.iFrameSpeed *
						  m_pFbxObj->m_AnimScene.iTickPerFrame;  // * 0.0f;

	if (m_pFbxObj->m_fTick >= m_pFbxObj->m_AnimScene.iEndFrame * m_pFbxObj->m_AnimScene.iTickPerFrame)
	{
		m_pFbxObj->m_fTick = 0;
	}

	for (int iNode = 0; iNode < m_pFbxObj->m_hNodeList.size() ; iNode++)
	{
		Matrix matWorld = Matrix::Identity;
		HModelObj* pModelObj = m_pFbxObj->m_hNodeList[iNode];

		for (int iTick = 0; iTick < pModelObj->m_AnimList.size(); iTick++)
		{
			if (pModelObj->m_AnimList[iTick].iTick >= m_pFbxObj->m_fTick)
			{
				matWorld = pModelObj->m_AnimList[iTick].mat;
				m_pFbxObj->m_MatrixList[iNode] = matWorld;
				break;
			}
		}
	}


	return true;
};

bool Sample::Render()
{

	for (int iNode = 0; iNode < m_pFbxObj->m_hNodeList.size(); iNode++)
	{
		Matrix matWorld = Matrix::Identity;
		HModelObj* pObject = m_pFbxObj->m_hNodeList[iNode];
		

		if (pObject->m_SubMesh.size() == 0)
		{
			if (pObject->m_TriangleList.size() <= 0) continue;

			pObject->m_cbData.vColor[0] = m_pMainCamera->m_vLook.x;
			pObject->m_cbData.vColor[1] = m_pMainCamera->m_vLook.y;
			pObject->m_cbData.vColor[2] = m_pMainCamera->m_vLook.z;
			pObject->m_matWorld = pObject->m_AnimList[0].mat;
			pObject->SetMatrix(NULL, &m_pMainCamera->m_matView,
				&m_pMainCamera->m_matProject);
			pObject->Render(g_pImmediateContext);
		}
		else
		{
			pObject->SetMatrix(NULL, &m_pMainCamera->m_matView,
				&m_pMainCamera->m_matProject);

			pObject->m_cbData.vColor[0] = m_pMainCamera->m_vLook.x;
			pObject->m_cbData.vColor[1] = m_pMainCamera->m_vLook.y;
			pObject->m_cbData.vColor[2] = m_pMainCamera->m_vLook.z;
			pObject->HDxObject::Update(g_pImmediateContext);
			pObject->PreRender(g_pImmediateContext);
			UINT iStride = sizeof(PNCT_VERTEX);
			UINT iOffset = 0;
			g_pImmediateContext->IASetIndexBuffer(pObject->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			g_pImmediateContext->IASetInputLayout(pObject->m_pInputLayout);
			g_pImmediateContext->VSSetConstantBuffers(0, 1, &pObject->m_ConstantBuffer);
			g_pImmediateContext->PSSetConstantBuffers(0, 1, &pObject->m_ConstantBuffer);
			g_pImmediateContext->VSSetShader(pObject->m_pVertexShader, NULL, 0);
			g_pImmediateContext->PSSetShader(pObject->m_pPixelShader, NULL, 0);
			g_pImmediateContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)pObject->m_iTopology);

			for (int iSub = 0; iSub < pObject->m_SubMesh.size(); iSub++)
			{
				HSubMesh* pMesh = &pObject->m_SubMesh[iSub];
				if (pMesh->m_TriangleList.size() <= 0) continue;
				g_pImmediateContext->IASetVertexBuffers(0, 1,
					pMesh->m_pVertexBuffer.GetAddressOf(), &iStride, &iOffset);
				if (pMesh->m_pTexture != nullptr)
				{
					g_pImmediateContext->PSSetShaderResources(0, 1,
						&pMesh->m_pTexture->m_pTextureSRV);
				}
				if (pObject->m_pIndexBuffer == nullptr)
				{
					g_pImmediateContext->Draw(pMesh->m_VertexList.size(), 0);
				}
				else
				{
					PostRender();
				}
			}
		}
	}
	

	return true;
};

bool Sample::Release()
{

	for (auto data : m_pFbxObj->m_hNodeList)
	{
		HModelObj* pObject = data;
		pObject->Release();
		delete pObject;
	}

	return true;
};
