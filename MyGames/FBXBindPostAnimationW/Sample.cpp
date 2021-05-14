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
	m_pNormalMap = g_TextureMgr.Load(g_pd3dDevice, L"../../Image/FBX_Image/T_Pack_01_D.dds");

	m_Camera.CreateViewMatrix({ 0,50,-50 }, { 0,0,0 });

	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_Camera.CreateProjectionMatrix(1, 5000, HBASIS_PI / 4.0f, fAspect);

	m_pFbxObj = make_shared<HFbxObj>();

	if (m_pFbxObj->Load("../../Image/FBX_Image/SM_Barrel.fbx"))
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

					/////////////////////////////////
					pSub->m_IWVertexList.resize(pSub->m_TriangleList.size() * 3);
					for (int iFace = 0; iFace < pSub->m_TriangleList.size(); iFace++)
					{
						int iIndex = iFace * 3;
						pSub->m_IWVertexList[iIndex + 0] =
							pSub->m_TriangleList[iFace].vIWVertex[0];
						pSub->m_IWVertexList[iIndex + 1] =
							pSub->m_TriangleList[iFace].vIWVertex[1];
						pSub->m_IWVertexList[iIndex + 2] =
							pSub->m_TriangleList[iFace].vIWVertex[2];

					}

					// vb
					ID3D11Buffer* vbiw =
						CreateVertexBuffer(g_pd3dDevice,
							&pSub->m_IWVertexList.at(0),
							pSub->m_IWVertexList.size(),
							sizeof(IW_VERTEX));
					pSub->m_pIWVertexBuffer.Attach(vbiw);


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

	D3D11_BUFFER_DESC vbdesc =
	{
		MAX_BONE_MATRICES * sizeof(Matrix),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, //D3D11_BIND_SHADER_RESOURCE,
		D3D11_CPU_ACCESS_WRITE,
		0
	};
	g_pd3dDevice->CreateBuffer(&vbdesc, NULL, m_pFbxObj->m_pBoneBuffer.GetAddressOf());

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

	// 상수버퍼 업데이트
	Matrix* pMatrices;
	HRESULT hr = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(g_pImmediateContext->Map((ID3D11Resource*)m_pFbxObj->m_pBoneBuffer.Get(), 0,
							D3D11_MAP_WRITE_DISCARD, 0, &MappedFaceDest)))
	{
		pMatrices = (Matrix*)MappedFaceDest.pData;
		for (int iObj = 0; iObj < m_pFbxObj->m_hNodeList.size(); iObj++)
		{
			Matrix matAnim = m_pFbxObj->m_MatrixList[iObj].Transpose();
			pMatrices[iObj] = matAnim;
		}
		g_pImmediateContext->Unmap(m_pFbxObj->m_pBoneBuffer.Get(), 0);
	}

	return true;
};

bool Sample::Render()
{
	if (m_pNormalMap != nullptr)
	{
		g_pImmediateContext->PSSetShaderResources(3, 1,
			&m_pNormalMap->m_pTextureSRV);
	}

	g_pImmediateContext->VSSetConstantBuffers(1, 1, m_pFbxObj->m_pBoneBuffer.GetAddressOf());

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

				ID3D11Buffer* vb[2] = { pMesh->m_pVertexBuffer.Get(), pMesh->m_pIWVertexBuffer.Get() };

				UINT iStride[2] = { sizeof(PNCT_VERTEX), sizeof(IW_VERTEX) };
				UINT iOffset[2] = { 0,0 };

				g_pImmediateContext->IASetVertexBuffers(0, 2, vb, iStride, iOffset);
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
