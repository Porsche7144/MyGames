#include "Sample.h"
#include "HTextureManager.h"
#include "HTimer.h"

LRESULT	 Sample::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pMainCamera == nullptr) return -1;
	m_pMainCamera->WndProc(hWnd, message, wParam, lParam);

	return -1;
}

bool Sample::Init()
{
	m_Camera.CreateViewMatrix({ 0,200,-200 }, { 0,0,0 });

	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_Camera.CreateProjectionMatrix(1, 5000, HBASIS_PI / 4.0f, fAspect);

	m_pFbxObj = make_shared<HFbxObj>();

	if (m_pFbxObj->Load("../../Image/FBX_Image/man.fbx"))
	{
		HStopwatch stopwatch;
		for (auto data : m_pFbxObj->m_hNodeList)
		{
			HModelObj* pObject = data;
			
			for (int iSub = 0; iSub < pObject->m_SubMesh.size(); iSub++)
			{
				HSubMesh* pSub = &pObject->m_SubMesh[iSub];
				if (pSub->m_iNumFace <= 0) continue;

				//pSub->m_VertexList.resize(
				//	pSub->m_TriangleList.size() * 3);
				//for (int iFace = 0; iFace < pSub->m_TriangleList.size(); iFace++)
				//{
				//	int iIndex = iFace * 3;
				//	pSub->m_VertexList[iIndex + 0] =
				//		pSub->m_TriangleList[iFace].vVertex[0];
				//	pSub->m_VertexList[iIndex + 1] =
				//		pSub->m_TriangleList[iFace].vVertex[1];
				//	pSub->m_VertexList[iIndex + 2] =
				//		pSub->m_TriangleList[iFace].vVertex[2];
				//}

				// 첫번째 PNCT vb
				ID3D11Buffer* vb =
					CreateVertexBuffer(g_pd3dDevice,
						&pSub->m_VertexList.at(0),
						pSub->m_VertexList.size(),
						sizeof(PNCT_VERTEX));
				pSub->m_pVertexBuffer.Attach(vb);

				//////////////////////////////////////////
				//pSub->m_IWVertexList.resize(pSub->m_TriangleList.size() * 3);
				//for (int iFace = 0; iFace < pSub->m_TriangleList.size(); iFace++)
				//{
				//	int iIndex = iFace * 3;
				//	pSub->m_IWVertexList[iIndex + 0] =
				//		pSub->m_TriangleList[iFace].vIWVertex[0];
				//	pSub->m_IWVertexList[iIndex + 1] =
				//		pSub->m_TriangleList[iFace].vIWVertex[1];
				//	pSub->m_IWVertexList[iIndex + 2] =
				//		pSub->m_TriangleList[iFace].vIWVertex[2];

				//}

				// 두번째 IW vb
				ID3D11Buffer* vbiw =
					CreateVertexBuffer(g_pd3dDevice,
						&pSub->m_IWVertexList.at(0),
						pSub->m_IWVertexList.size(),
						sizeof(IW_VERTEX));
				pSub->m_pIWVertexBuffer.Attach(vbiw);

				// 인덱스 버퍼
				ID3D11Buffer* ib =
					CreateIndexBuffer(g_pd3dDevice,
						&pSub->m_IndexArray.at(0),
						pSub->m_IndexArray.size(),
						sizeof(DWORD));
				pSub->m_pIndexBuffer.Attach(ib);


				wstring loadTex = L"../../Image/FBX_Image/";
				loadTex += pObject->FbxMaterialList[iSub].c_str();
				pSub->m_pTexture = g_TextureMgr.Load(g_pd3dDevice, loadTex.c_str());

			}

		}
		if (!m_pFbxObj->Create(g_pd3dDevice,
			L"VS.txt",
			L"PS.txt",
			L""))
		{
			return false;
		}
		stopwatch.Output(L"Convert");
	}

	D3D11_BUFFER_DESC vbdesc =
	{
		// 실시간 적용을 위해 D3D11_USAGE_DYNAMIC
		MAX_BONE_MATRICES * sizeof(Matrix),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, //D3D11_BIND_SHADER_RESOURCE,
		D3D11_CPU_ACCESS_WRITE,
		0
	};
	g_pd3dDevice->CreateBuffer(&vbdesc, NULL, m_pFbxObj->m_pBoneBuffer.GetAddressOf());

	m_pFbxObj->m_fTick = 3 * 160;

	return true;
};

bool Sample::Frame()
{
	m_pFbxObj->m_fTick += g_fSecondPerFrame * m_pFbxObj->m_AnimScene.iFrameSpeed *
		m_pFbxObj->m_AnimScene.iTickPerFrame * 1.0f; //* 0.0f;

	if (m_pFbxObj->m_fTick >= m_pFbxObj->m_AnimScene.iEndFrame * m_pFbxObj->m_AnimScene.iTickPerFrame)
	{
		m_pFbxObj->m_fTick = 3 * 160;
	}

	for (int iNode = 0; iNode < m_pFbxObj->m_hNodeList.size(); iNode++)
	{
		HModelObj* pModelObj = m_pFbxObj->m_hNodeList[iNode];
		// matWorld = Matrix::Identity;

		// 바이패드 공간 * Global
		std::string szName;
		szName.assign(pModelObj->m_szName.begin(), pModelObj->m_szName.end());
		Matrix matBiped = Matrix::Identity;

		auto data = m_pFbxObj->m_dxMatrixMap.find(szName);
		if (data != m_pFbxObj->m_dxMatrixMap.end())
		{
			matBiped = data->second;
		}
		
		//matParent = Matrix::Identity;
		if (pModelObj->m_pParentObj != nullptr)
		{
			matParent = pModelObj->m_pParentObj->m_matAnim;
		}
		else
		{
			matParent = Matrix::Identity;
			matParent._41 = matWorld._41;
			matParent._42 = matWorld._42;
			matParent._43 = matWorld._43;
		}

		for (int iTick = 1; iTick < pModelObj->m_AnimList.size()-1; iTick++)
		{

			if (pModelObj->m_AnimList[iTick].iTick >= m_pFbxObj->m_fTick)
			{
				// 이전 프레임과 다음 프레임 보간을 위해
				// 프레임간의 틱 계산
				int iStart = pModelObj->m_AnimList[iTick - 1].iTick;
				int iEnd = pModelObj->m_AnimList[iTick].iTick;
				int iStepTick = iEnd - iStart;
				float t = (m_pFbxObj->m_fTick - iStart) / iStepTick;
				
				// 보간
				Vector3 vStart, vEnd, vPos;
				vStart = pModelObj->m_AnimList[iTick - 1].p;
				vEnd = pModelObj->m_AnimList[iTick].p;
				vPos = Vector3::Lerp(vStart, vEnd, t);

				Vector3 vScale;
				vStart = pModelObj->m_AnimList[iTick - 1].s;
				vEnd = pModelObj->m_AnimList[iTick].s;
				vScale = Vector3::Lerp(vStart, vEnd, t);

				Quaternion q1, q2, qRot;
				q1 = pModelObj->m_AnimList[iTick - 1].q;
				q2 = pModelObj->m_AnimList[iTick].q;
				qRot = Quaternion::Slerp(q1, q2, t);

				// srt 
				Matrix matScale = Matrix::CreateScale(vScale);
				Matrix matRotate = Matrix::CreateFromQuaternion(qRot);
				Matrix matTrans = Matrix::CreateTranslation(vPos);
				pModelObj->m_matAnim = matScale * matRotate * matTrans * matParent;

				// matWorld = pModelObj->m_AnimList[iTick].mat;
				m_pFbxObj->m_MatrixList[iNode] = /*matBiped */ pModelObj->m_matAnim;
				break;
			}
		}

		if (g_Input.GetKey(VK_UP) == KEY_HOLD)
		{
			matWorld._43 = matWorld._43 + (1.0f * g_fSecondPerFrame);
			if (m_pFbxObj->m_fTick >= 33 * m_pFbxObj->m_AnimScene.iTickPerFrame)
			{
				m_pFbxObj->m_fTick = 7 * 160;
			}
			// matWorld._43 = pModelObj->m_matAnim._43;
		}
		if (g_Input.GetKey(VK_DOWN) == KEY_HOLD)
		{
			matWorld._43 = matWorld._43 - (1.0f * g_fSecondPerFrame);
			if (m_pFbxObj->m_fTick >= 33 * m_pFbxObj->m_AnimScene.iTickPerFrame)
			{
				m_pFbxObj->m_fTick = 7 * 160;
			}
			// matWorld._43 = pModelObj->m_matAnim._43;
		}
		if (g_Input.GetKey(VK_RIGHT) == KEY_HOLD)
		{
			matWorld._41 = matWorld._41 + (1.0f * g_fSecondPerFrame);
			// matWorld._41 = pModelObj->m_matAnim._41;
		}
		if (g_Input.GetKey(VK_LEFT) == KEY_HOLD)
		{
			matWorld._41 = matWorld._41 - (1.0f * g_fSecondPerFrame);
			// matWorld._41 = pModelObj->m_matAnim._41;
		}

		if (g_Input.GetKey(VK_SPACE) == KEY_HOLD)
		{
 			if (m_pFbxObj->m_fTick >= 855 * m_pFbxObj->m_AnimScene.iTickPerFrame)
			{
				m_pFbxObj->m_fTick = 840 * 160;
			}
		}

		 matParent._41 = matWorld._41;
		 matParent._42 = matWorld._42;
		 matParent._43 = matWorld._43;			
	}

	return true;
};

bool Sample::Render()
{

	   
	m_pFbxObj->SetMatrix(&matWorld, &m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProject);

	m_pFbxObj->m_cbData.vColor[0] = m_pMainCamera->m_vLook.x;
	m_pFbxObj->m_cbData.vColor[1] = m_pMainCamera->m_vLook.y;
	m_pFbxObj->m_cbData.vColor[2] = m_pMainCamera->m_vLook.z;
	m_pFbxObj->HDxObject::Update(g_pImmediateContext);
	m_pFbxObj->PreRender(g_pImmediateContext);

	g_pImmediateContext->VSSetConstantBuffers(1, 1, m_pFbxObj->m_pBoneBuffer.GetAddressOf());
	g_pImmediateContext->IASetInputLayout(m_pFbxObj->m_pInputLayout);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pFbxObj->m_ConstantBuffer);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &m_pFbxObj->m_ConstantBuffer);
	g_pImmediateContext->VSSetShader(m_pFbxObj->m_pVertexShader, NULL, 0);
	g_pImmediateContext->PSSetShader(m_pFbxObj->m_pPixelShader, NULL, 0);
	g_pImmediateContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_pFbxObj->m_iTopology);

	for (int iNode = 0; iNode < m_pFbxObj->m_hNodeList.size(); iNode++)
	{
		Matrix matWorld = Matrix::Identity;
		HModelObj* pObject = m_pFbxObj->m_hNodeList[iNode];	
		if (pObject->m_dxMatrixBindPoseMap.size() <= 0)
		{
			continue;
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
				HModelObj* bone = m_pFbxObj->m_hNodeList[iObj];
				std::string szName;
				szName.assign(bone->m_szName.begin(), bone->m_szName.end());
				Matrix matBiped = Matrix::Identity;
				auto data = pObject->m_dxMatrixBindPoseMap.find(szName);
				if (data != pObject->m_dxMatrixBindPoseMap.end())
				{
					matBiped = data->second;
				}

				Matrix matAnim = matBiped * m_pFbxObj->m_MatrixList[iObj];
				pMatrices[iObj] = matAnim.Transpose();
			}
			g_pImmediateContext->Unmap(m_pFbxObj->m_pBoneBuffer.Get(), 0);
		}

		for (int iSub = 0; iSub < pObject->m_SubMesh.size(); iSub++)
		{
			HSubMesh* pMesh = &pObject->m_SubMesh[iSub];
			if (pMesh->m_iNumFace <= 0) continue;

			//////////////////////////////
			// 멀티스트림 = 버퍼 다중사용.
			ID3D11Buffer* vb[2] = { pMesh->m_pVertexBuffer.Get(), pMesh->m_pIWVertexBuffer.Get() };
			UINT iStride[2] = { sizeof(PNCT_VERTEX), sizeof(IW_VERTEX) };
			UINT iOffset[2] = { 0,0 };

			g_pImmediateContext->IASetVertexBuffers(0, 2, vb, iStride, iOffset);
			g_pImmediateContext->IASetIndexBuffer(pMesh->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

			if (pMesh->m_pTexture != nullptr)
			{
				g_pImmediateContext->PSSetShaderResources(0, 1,	&pMesh->m_pTexture->m_pTextureSRV);
			}
			else
			{
				g_pImmediateContext->PSSetShaderResources(0, 1,	&g_TextureMgr.m_pWhiteTexture->m_pTextureSRV);
			}
			if (pMesh->m_pIndexBuffer == nullptr)
			{
				g_pImmediateContext->Draw(pMesh->m_VertexList.size(), 0);
			}
			else
			{
				g_pImmediateContext->DrawIndexed(pMesh->m_IndexArray.size(), 0, 0);
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
	m_pFbxObj->Release();

	return true;
};

////////
// 1. 사용하지 않는 s r t를 제외하여 연산량 줄이기
// 2. 월드행렬 넘겨서 애니메이션 이동
// 3. 캐릭터 동작별로 나누는 함수, 동작과 동작 보간