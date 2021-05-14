#include "Sample.h"

int Sample::m_iTileCount = 257;
float Sample::m_fCellCount = 10.0f;
float Sample::m_fScale = 10.0f;

LRESULT	 Sample::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pMainCamera == nullptr) return -1;
	m_pMainCamera->WndProc(hWnd, message, wParam, lParam);

	return -1;
}

bool Sample::GetIntersection(HNode* pNode)
{
	// face list
	for (int face = 0; face < pNode->m_IndexList.size() / 3; face++)
	{
		v0 = m_Map.m_VertexList[pNode->m_IndexList[face * 3 + 0]].p;
		v1 = m_Map.m_VertexList[pNode->m_IndexList[face * 3 + 1]].p;
		v2 = m_Map.m_VertexList[pNode->m_IndexList[face * 3 + 2]].p;

		vEnd = m_Picking.m_Ray.vOrigin + m_Picking.m_Ray.vDir * 10000.0f;
		vNormal = (v1 - v0).Cross(v2 - v0);
		vNormal.Normalize();

		// 방법 1) 외적을 사용하는 방법
		if (m_Picking.GetIntersection(m_Picking.m_Ray.vOrigin, vEnd, vNormal, v0, v1, v2))
		{
			if (m_Picking.PointInPolygon(m_Picking.m_vInterSection, vNormal, v0, v1, v2))
			{
				list[0] = v0;
				list[1] = v1;
				list[2] = v2;

				return true;
			}
		}

		// 방법 2) UV 매개변수를 사용한 교점과 교점 포함 테스트를
		//         동시에 처리 하는 방법(평면 노말 필요X).
		//if (m_Picking.IntersectTriangle(vPickRayOrigin, vPickRayDir, v0, v1, v2, t, u, v))
		//{
		//	list[0] = v0;
		//	list[1] = v1;
		//	list[2] = v2;
		//	return true;
		//}
	}

	return false;
}

TCHAR* Sample::StringToTCHAR(string& s)
{
	string tstr;
	const char* all = s.c_str();
	int len = 1 + strlen(all);
	wchar_t* t = new wchar_t[len];
	if (NULL == t) throw std::bad_alloc();
	mbstowcs(t, all, len);
	return (TCHAR*)t;
}

bool Sample::Init()
{
	HRESULT hr;
	TCHAR* Filename = StringToTCHAR(m_FileName);

	m_Camera.CreateViewMatrix({ 0,300,-100 }, { 0,0,0 });
	m_pObj.Init();

	/////////////////////////////////////////////////////////////////////////////
	// 유저 생성
	/////////////////////////////////////////////////////////////////////////////
	if (!m_UserShape.Create(g_pd3dDevice, L"VS.txt", L"PS.txt", L"../../Image/tileA.jpg"))
	{
		return false;
	}

	/////////////////////////////////////////////////////////////////////////////
	// 높이맵 생성
	/////////////////////////////////////////////////////////////////////////////
	//if (!m_Map.CreateHeightMap(g_pd3dDevice, g_pImmediateContext, L"../../Image/data/map/129.jpg"))
	//{
	//	return false;
	//}

	HMapDesc desc;
	desc.iNumCols = m_iTileCount;
	desc.iNumRows = m_iTileCount;
	desc.fCellDistance = m_fCellCount;
	desc.fScaleHeight = m_fScale;
	desc.szTextFile = Filename;
	desc.szVS = L"../../data/Shader/ToolBaseVS.txt";
	desc.szPS = L"../../data/Shader/ToolBasePS.txt";
    m_Map.CreateMap(g_pd3dDevice, desc);

	m_vDirValue = { 0,0,0,0 };

	if (!m_Minimap.Create(g_pd3dDevice, L"VS.txt", L"PS.txt", Filename))
	{
		return false;
	}

	if (!m_TextureMap.Create(&m_Map, g_pd3dDevice, g_pImmediateContext, L"HeightMapVS.txt", L"HeightMapPS.txt", L""))
	{
		return false;
	}

	if (!m_AlphaZeroTexture.Create(&m_Map, g_pd3dDevice, g_pImmediateContext, L"HeightMapVS.txt", L"HeightMapPS.txt", Filename))
	{
		return false;
	}

	//m_pTextureSRV[0] = CreateShaderResourceView(g_pd3dDevice, L"../../Image/data/map/001.jpg");
	//m_pTextureSRV[0] = m_AlphaZeroTexture.m_pSRV;

	m_pTextureSRV[1] = CreateShaderResourceView(g_pd3dDevice, L"../../Image/data/map/002.jpg");
	m_pTextureSRV[2] = CreateShaderResourceView(g_pd3dDevice, L"../../Image/data/map/003.jpg");
	m_pTextureSRV[3] = CreateShaderResourceView(g_pd3dDevice, L"../../Image/data/map/004.jpg");
	m_pTextureSRV[4] = CreateShaderResourceView(g_pd3dDevice, L"../../Image/data/map/005.jpg");

	// 쿼드트리 공간분할
	m_QuadTree.Build(&m_Map);


	// 카메라 바꿔치는 부분.
	m_ModelCamera.CreateViewMatrix({ 0,30,-30 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_ModelCamera.CreateProjectionMatrix(1, 10000, HBASIS_PI / 4.0f, fAspect);
	m_ModelCamera.Init();

	// 프러스텀 생성
	//m_ModelCamera.CreateFrustum(g_pd3dDevice, g_pImmediateContext);
	//m_pMainCamera = &m_ModelCamera;

	m_pObj.m_pMainCamera = m_pMainCamera;


	m_TopCamera.CreateViewMatrix({ 0,30,-0.1f }, { 0,0,0 });
	fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	int x = desc.iNumCols * m_fCellCount;
	int y = desc.iNumRows * m_fCellCount;
	m_TopCamera.CreateOrthographic(x, y, 1, 10000);
	m_TopCamera.Init();

	HCore::m_bFrameRun = true;

	/*m_TopCamera.CreateViewMatrix({ 0,30,-0.1f }, { 0,0,0 });
	fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_TopCamera.CreateOrthographic(desc.iNumCols, desc.iNumRows, 1.0f, 1000);
	m_TopCamera.Init();*/



	// 1024 * 1024

	return true;
}

bool Sample::Frame()
{
	m_pObj.Frame();

	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		m_TextureMap.Frame(&m_Map, g_pImmediateContext);
	}

	if (g_Input.GetKey('9') == KEY_PUSH)
	{
		m_AlphaZeroTexture.WriteTextureDataAlphaZero(&m_Map, g_pImmediateContext);
		g_pImmediateContext->CopyResource(m_AlphaZeroTexture.pTexture, m_AlphaZeroTexture.pStaging);
	}
	if (g_Input.GetKey('8') == KEY_PUSH)
	{
		m_AlphaZeroTexture.PickRenderTextureData(&m_Map, m_AlphaZeroTexture.pStaging ,g_pImmediateContext);
		g_pImmediateContext->CopyResource(m_AlphaZeroTexture.pTexture, m_AlphaZeroTexture.pStaging);
		m_pTextureSRV[0] = m_AlphaZeroTexture.m_pSRV;
	}



	m_bSelect = false;

#pragma region Mouse Picking
	if (g_Input.GetKey(VK_LBUTTON) == KEY_HOLD && g_Input.GetKey(VK_SHIFT))
	{
		m_bSelect = true;
		POINT cursor;
		GetCursorPos(&cursor);
		ScreenToClient(g_hWnd, &cursor);
		Matrix matProj = m_pMainCamera->m_matProject;

		Vector3 vec;
		// 화면 -> 투영 -> 뷰
		vec.x = (((2.0f*cursor.x) / g_rtClient.right) - 1) / matProj._11;
		vec.y = -(((2.0f*cursor.y) / g_rtClient.bottom) - 1) / matProj._22;
		vec.z = 1.0f;

		Matrix matInvView = m_pMainCamera->m_matView;
		matInvView = matInvView.Invert();

		// 방법 1) 직접 정점 변환
		vPickRayDir.x = vec.x * matInvView._11 + vec.y * matInvView._21 + vec.z * matInvView._31;
		vPickRayDir.y = vec.x * matInvView._12 + vec.y * matInvView._22 + vec.z * matInvView._32;
		vPickRayDir.z = vec.x * matInvView._13 + vec.y * matInvView._23 + vec.z * matInvView._33;
		vPickRayDir.Normalize();

		vPickRayOrigin.x = matInvView._41;
		vPickRayOrigin.y = matInvView._42;
		vPickRayOrigin.z = matInvView._43;

		// 방법 2) 함수 사용 정점 변환
		//vPickRayOrigin = Vector3{ 0.0f, 0.0f, 0.0f };
		//vPickRayDir = vec;
		//vPickRayOrigin = Vector3::Transform(vPickRayOrigin, matInvView);
		//vPickRayDir = Vector3::TransformNormal(vPickRayDir, matInvView);
		//vPickRayDir.Normalize();

		m_Picking.m_Ray.vDir = vPickRayDir;
		m_Picking.m_Ray.vOrigin = vPickRayOrigin;

	}
#pragma endregion


/*
	if (g_Input.GetKey('W') == KEY_HOLD)
	{
		m_UserShape.FrontMovement(1.0f);
	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_UserShape.FrontMovement(-1.0f);
	}
	if (g_Input.GetKey('A') == KEY_HOLD)
	{
		m_UserShape.RightMovement(-1.0f);
	}
	if (g_Input.GetKey('D') == KEY_HOLD)
	{
		m_UserShape.RightMovement(1.0f);
	}
	if (g_Input.GetKey('Q') == KEY_HOLD)
	{
		m_UserShape.UpMovement(1.0f);
	}
	if (g_Input.GetKey('E') == KEY_HOLD)
	{
		m_UserShape.UpMovement(-1.0f);
	}*/


	m_UserShape.Frame();

	m_pMainCamera->m_vCameraTarget = m_UserShape.m_vPos;
	//m_pMainCamera->FrameFrustum(g_pImmediateContext);

	m_UserShape.m_matRotation = m_pMainCamera->m_matWorld;


	return true;
}

bool Sample::Render()
{

#pragma region ModelRender
	for (auto data : m_ModelMatrixList)
	{
		m_pObj.m_pFbxObj->SetMatrix(&data, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
		//m_pObj.Render(g_pImmediateContext);
	}
#pragma endregion

#pragma region Rasterize
	if (g_Input.GetKey('1') == KEY_PUSH)
	{
		HDxState::g_RasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		HDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext, HDxState::g_RasterizerDesc);
		ApplyRS(g_pImmediateContext, HDxState::g_pRSEdit.Get());
	}

	if (g_Input.GetKey('2') == KEY_PUSH)
	{
		HDxState::g_RasterizerDesc.FillMode = D3D11_FILL_SOLID;
		HDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext, HDxState::g_RasterizerDesc);
		ApplyRS(g_pImmediateContext, HDxState::g_pRSEdit.Get());
	}

	if (g_Input.GetKey('3') == KEY_PUSH)
	{
		HDxState::g_RasterizerDesc.CullMode = D3D11_CULL_BACK;
		HDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext, HDxState::g_RasterizerDesc);
		ApplyRS(g_pImmediateContext, HDxState::g_pRSEdit.Get());
	}

	if (g_Input.GetKey('4') == KEY_PUSH)
	{
		HDxState::g_RasterizerDesc.CullMode = D3D11_CULL_FRONT;
		HDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext, HDxState::g_RasterizerDesc);
		ApplyRS(g_pImmediateContext, HDxState::g_pRSEdit.Get());
	}

	if (g_Input.GetKey('W') == KEY_HOLD)
	{
		m_pMainCamera->FrontMovement(20.0f);

	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_pMainCamera->FrontMovement(-20.0f);
	}
	if (g_Input.GetKey('A') == KEY_HOLD)
	{
		m_pMainCamera->RightMovement(-20.0f);
	}
	if (g_Input.GetKey('D') == KEY_HOLD)
	{
		m_pMainCamera->RightMovement(20.0f);
	}
	if (g_Input.GetKey('Q') == KEY_HOLD)
	{
		m_pMainCamera->UpMovement(20.0f);
	}
	if (g_Input.GetKey('E') == KEY_HOLD)
	{
		m_pMainCamera->UpMovement(-20.0f);
	}
#pragma endregion

#pragma region Field Update
	m_SelectNode.clear();

	if (m_bSelect)
	{
		for (int i = 0; i < m_QuadTree.m_leafList.size(); i++)
		{
			HNode* pNode = m_QuadTree.m_leafList[i];

			if (m_Picking.IntersectBox(&pNode->m_hBox, &m_Picking.m_Ray))
			{
				m_SelectNode.push_back(pNode);
			}
		}

		Vector3 pick;
		float fMaxDist = 99999;
		bool Update = false;
		for (int select = 0; select < m_SelectNode.size(); select++)
		{
			HNode* pNode = m_SelectNode[select];
			if (GetIntersection(pNode))
			{
				float fDistance = (m_Picking.m_Ray.vOrigin - m_Picking.m_vInterSection).Length();
				if (fMaxDist > fDistance)
				{
					Update = true;
					pick = m_Picking.m_vInterSection;
					fMaxDist = fDistance;
				}
			}

		}
		//if (Update)
		//{
		//	Matrix matWorld;
		//	matWorld._41 = m_Picking.m_vInterSection.x;
		//	matWorld._42 = m_Picking.m_vInterSection.y;
		//	matWorld._43 = m_Picking.m_vInterSection.z;
		//	m_ModelMatrixList.push_back(matWorld);
		//}

		m_ControlNode.clear();
		H_SPHERE pickSphere;
		pickSphere.vCenter = pick;
		pickSphere.fRadius = 50.0f;

		for (int i = 0; i < m_QuadTree.m_leafList.size(); i++)
		{
			HNode* pNode = m_QuadTree.m_leafList[i];
			if (HCollision::SphereToSphere(pNode->m_Sphere, pickSphere))
			{
				m_ControlNode.push_back(pNode);
			}
		}

		
		if (m_bIncreaseGround)
		{
			for (auto node : m_ControlNode)
			{
				for (int v = 0; v < node->m_IndexList.size(); v++)
				{
					int iID = node->m_IndexList[v];
					float fDist = (m_Map.m_VertexList[iID].p - pick).Length();
					if (fDist < m_fRadius)
					{
						if (m_Map.m_VertexList[iID].p.y < 255)
						{
							Vector3 v = m_Map.m_VertexList[iID].p;
							m_Map.m_VertexList[iID].p.y = v.y + m_fSpeed - sinf((fDist / m_fRadius));

						}
						beforePos.y = pick.y;
					}
				}

				// 실시간 노말 계산
				//m_Map.CalcPerVertexNormalsFastLookUp();
			}
		}

		if (m_bDecreaseGround)
		{
			for (auto node : m_ControlNode)
			{
				for (int v = 0; v < node->m_IndexList.size(); v++)
				{
					int iID = node->m_IndexList[v];
					float fDist = (m_Map.m_VertexList[iID].p - pick).Length();
					if (fDist < m_fRadius)
					{
						if (m_Map.m_VertexList[iID].p.y > -255)
						{
							Vector3 v = m_Map.m_VertexList[iID].p;
							m_Map.m_VertexList[iID].p.y = v.y - m_fSpeed - sinf((fDist / m_fRadius));
							beforePos.y = pick.y;
						}						
					}
				}

				// 실시간 노말 계산
				//m_Map.CalcPerVertexNormalsFastLookUp();
			}
		}


		if (m_bOriginGround)
		{
			for (auto node : m_ControlNode)
			{
				for (int v = 0; v < node->m_IndexList.size(); v++)
				{
					int iID = node->m_IndexList[v];
					float fDist = (m_Map.m_VertexList[iID].p - pick).Length();
					beforePos.y = 0.0f;
					if (fDist < m_fRadius)
					{
						if (m_Map.m_VertexList[iID].p.y >= 0)
						{
							Vector3 v = m_Map.m_VertexList[iID].p;
							m_Map.m_VertexList[iID].p.y = beforePos.y - m_fSpeed - sinf((fDist / m_fRadius));
						}
						if (m_Map.m_VertexList[iID].p.y <= 0)
						{
							Vector3 v = m_Map.m_VertexList[iID].p;
							m_Map.m_VertexList[iID].p.y = beforePos.y + m_fSpeed - sinf((fDist / m_fRadius));
						}
					}
				}

				// 실시간 노말 계산
				//m_Map.CalcPerVertexNormalsFastLookUp();
			}
		}

		if (m_bFlatGrond)
		{
			for (auto node : m_ControlNode)
			{
				for (int v = 0; v < node->m_IndexList.size(); v++)
				{
					int iID = node->m_IndexList[v];
					float fDist = (m_Map.m_VertexList[iID].p - pick).Length();
					if (fDist < m_fRadius)
					{
						if (m_Map.m_VertexList[iID].p.y >= beforePos.y || m_Map.m_VertexList[iID].p.y <= beforePos.y)
						{
							Vector3 v = m_Map.m_VertexList[iID].p;
							m_Map.m_VertexList[iID].p.y = beforePos.y + m_fSpeed - sinf((fDist / m_fRadius));
						}
					}
				}

				// 실시간 노말 계산
				//m_Map.CalcPerVertexNormalsFastLookUp();
			}
		}


		if (m_bFlatGrond)
		{
			for (auto node : m_ControlNode)
			{
				for (int v = 0; v < node->m_IndexList.size(); v++)
				{
					int iID = node->m_IndexList[v];
					float fDist = (m_Map.m_VertexList[iID].p - pick).Length();
					if (fDist < m_fRadius)
					{
						
					}
				}

				// 실시간 노말 계산
				//m_Map.CalcPerVertexNormalsFastLookUp();
			}
		}



		g_pImmediateContext->UpdateSubresource(m_Map.m_pVertexBuffer, 0, NULL, &m_Map.m_VertexList.at(0), 0, 0);
	}
#pragma endregion

#pragma region MapRender
	m_Map.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
	// m_Map.Render(g_pImmediateContext);
	m_Map.m_cbData.vColor[0] = m_pMainCamera->m_vLook.x;
	m_Map.m_cbData.vColor[1] = m_pMainCamera->m_vLook.y;
	m_Map.m_cbData.vColor[2] = m_pMainCamera->m_vLook.z;

	for (auto node : m_QuadTree.m_leafList)
	{
		g_pImmediateContext->PSSetShaderResources(1, 1, &m_pTextureSRV[0]);
		g_pImmediateContext->PSSetShaderResources(2, 4, &m_pTextureSRV[1]);
		m_QuadTree.Draw(g_pImmediateContext, node);		
	}
#pragma endregion

#pragma region MiniMapRender
	m_Minimap.SetMatrix(NULL, NULL, NULL);
	m_Minimap.Begin(g_pImmediateContext);
	m_Map.SetMatrix(NULL, &m_TopCamera.m_matView, &m_TopCamera.m_matProject);
	for (auto node : m_QuadTree.m_leafList)
	{
		g_pImmediateContext->PSSetShaderResources(1, 1, &m_pTextureSRV[0]);
		g_pImmediateContext->PSSetShaderResources(2, 4, &m_pTextureSRV[1]);
		m_QuadTree.Draw(g_pImmediateContext, node);
	}	
	m_Map.Render(g_pImmediateContext);
	m_Minimap.End(g_pImmediateContext);
#pragma endregion	

	Matrix mat;
	mat._41 = 1.5f;

	Matrix matOrtho;
	matOrtho._41 = 0.75f;
	m_AlphaZeroTexture.SetMatrix(NULL, NULL, &matOrtho);
	m_AlphaZeroTexture.Render(g_pImmediateContext);

	m_UserShape.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
	m_UserShape.Render(g_pImmediateContext);

	m_Minimap.SetMatrix(NULL, NULL, NULL);
	m_Minimap.Render(g_pImmediateContext);

	m_TextureMap.SetMatrix(NULL, NULL, &mat);
	m_TextureMap.Render(g_pImmediateContext);

	m_LineShape.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
	m_LineShape.Draw(g_pImmediateContext, vPickRayOrigin, vPickRayOrigin + vPickRayDir * 100.0f);
	m_LineShape.Draw(g_pImmediateContext, list[0], list[1]);
	m_LineShape.Draw(g_pImmediateContext, list[1], list[2]);
	m_LineShape.Draw(g_pImmediateContext, list[2], list[0]);

	return true;
}

bool Sample::PostRender()
{
	HCore::PostRender();
	return true;
}

bool Sample::Release()
{
	if (m_pTextureSRV)
	{
		m_pTextureSRV[0]->Release();
		m_pTextureSRV[1]->Release();
		m_pTextureSRV[2]->Release();
		m_pTextureSRV[3]->Release();
	}

	m_pObj.Release();
	m_QuadTree.Release();
	m_Minimap.Release();
	m_Map.Release();
	m_UserShape.Release();

	return true;
}


void HBoxUser::FrontMovement(float fDir)
{
	Vector3 vFrontMove = m_vLook * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vPos += vFrontMove;
}

void HBoxUser::RightMovement(float fDir)
{
	Vector3 vRightMove = m_vRight * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vPos += vRightMove;
}

void HBoxUser::UpMovement(float fDir)
{
	Vector3 vUpMove = m_vUp * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vPos += vUpMove;
}

void HBoxUser::FrontBase(float fDir)
{
	Vector3 vLook = { 0,0,1 };
	Vector3 vMove = vLook * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vPos += vMove;
	m_vTarget += m_vLook * m_fSpeed;
}

void HBoxUser::RightBase(float fDir)
{
	Vector3 vSide = { 1,0,0 };
	Vector3 vMove = vSide * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vPos += vMove;
	m_vTarget += m_vLook * m_fSpeed;
}

void HBoxUser::UpBase(float fDir)
{
	Vector3 vUp = { 0,1,0 };
	Vector3 vMove = vUp * g_fSecondPerFrame * m_fSpeed * fDir;
	m_vPos += vMove;
	m_vTarget += m_vLook * m_fSpeed;
}















//#include "Sample.h"
//
//int Sample::m_iTileCount = 32;
//
//bool Sample::Init()
//{
//
//	HMapDesc MapDesc;
//	MapDesc.iNumCols = m_iTileCount;
//	MapDesc.iNumRows = m_iTileCount;
//	MapDesc.fCellDistance = 1.0f;
//	MapDesc.fScaleHeight = 0.1f;
//	MapDesc.szTextFile = L"../../Image/data/map/castle.jpg";
//	MapDesc.szVS = L"VS.txt";
//	MapDesc.szPS = L"PS.txt";
//	if (!m_CustomMap.CreateMap(g_pd3dDevice, MapDesc))
//	{
//		return false;
//	}
//
//	HCore::m_bFrameRun = true;
//
//	return true;
//}
//
//bool Sample::Frame()
//{
//
//	/*float t = cosf(g_fGameTimer * 0.5f);
//	for (int z = 0; z < m_CustomMap.m_iNumRows; z++)
//	{
//		for (int x = 0; x < m_CustomMap.m_iNumCols; x++)
//		{
//			int iIndex = z * m_CustomMap.m_iNumCols + x;
//			float fCos = cosf(t * x);
//			float fSin = sinf(t * z);
//			m_CustomMap.m_VertexList[iIndex].p.y = fCos + fSin;
//		}
//	}*/
//
//	// UpdateSubresource로 버퍼 업데이트
//	g_pImmediateContext->UpdateSubresource(m_CustomMap.m_pVertexBuffer, 0, NULL, &m_CustomMap.m_VertexList.at(0), 0, 0);
//	// m_pMainCamera->Frame();
//	m_CustomMap.Frame();
//
//	return true;
//}
//
//bool Sample::Render()
//{
//
//	m_CustomMap.SetMatrix(&m_pMainCamera->m_matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
//	m_CustomMap.Render(g_pImmediateContext);
//
//	return true;
//}
//
//
//bool Sample::Release()
//{
//	m_CustomMap.Release();
//	return true;
//}
//
//Sample::Sample()
//{
//	m_pMainCamera = nullptr;
//}
//Sample::~Sample()
//{
//
//}