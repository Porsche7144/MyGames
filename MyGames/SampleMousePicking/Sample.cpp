#include "Sample.h"

LRESULT	 Sample::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pMainCamera == nullptr) return -1;
	m_pMainCamera->WndProc(hWnd, message, wParam, lParam);

	return -1;
}

bool Sample::Init()
{
	HRESULT hr;

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
	if (!m_Map.CreateHeightMap(g_pd3dDevice, g_pImmediateContext, L"../../Image/data/map/129.jpg"))
	{
		return false;
	}

	HMapDesc desc;
	desc.iNumCols = m_Map.m_iNumCols;
	desc.iNumRows = m_Map.m_iNumRows;
	desc.fCellDistance = 1.0f;
	desc.fScaleHeight = 10.0f;
	desc.szTextFile = L"../../Image/data/map/grass_2.jpg";
	desc.szVS = L"VS.txt";
	desc.szPS = L"PS.txt";
	m_Map.CreateMap(g_pd3dDevice, desc);

	m_vDirValue = { 0,0,0,0 };

	/////////////////////////////////////////////////////////////////////////////
	// 미니맵 생성
	/////////////////////////////////////////////////////////////////////////////
	m_Minimap.Create(g_pd3dDevice, L"VS.txt", L"PS.txt", L"../../Image/tileA.jpg");

	Matrix matScale, matRotation;
	matScale = Matrix::CreateScale(100, 100, 0);
	matRotation = Matrix::CreateRotationX(HBASIS_PI * 0.5f);
	m_matPlaneWorld = matScale * matRotation;

	if (!m_BoxShape.Create(g_pd3dDevice, L"VS.txt", L"PS.txt", L"../../Image/tileA.jpg"))
	{
		return false;
	}

	// 쿼드트리 공간분할
	m_QuadTree.Build(&m_Map);

	if (!m_PlaneShape.Create(g_pd3dDevice, L"VS.txt", L"PS.txt", L"../../Image/tileA.jpg"))
	{
		return false;
	}

	
	// 카메라 바꿔치는 부분.
	m_ModelCamera.CreateViewMatrix({ 0,10,-10 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_ModelCamera.CreateProjectionMatrix(1, 150, HBASIS_PI / 4.0f, fAspect);
	m_ModelCamera.Init();

	// 프러스텀 생성
	m_ModelCamera.CreateFrustum(g_pd3dDevice, g_pImmediateContext);
	m_pMainCamera = &m_ModelCamera;

	m_TopCamera.CreateViewMatrix({ 0,30,-0.1f }, { 0,0,0 });
	fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_TopCamera.CreateOrthographic(desc.iNumCols, desc.iNumRows, 1.0f, 1000);
	m_TopCamera.Init();


	return true;
}

bool Sample::Frame()
{

	// Mouse Picking
	if (g_Input.GetKey(VK_LBUTTON))
	{
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
		//vPickRayDir = v;
		//vPickRayOrigin = Vector3::Transform(vPickRayOrigin, matInvView);
		//vPickRayDir = Vector3::TransformNormal(vPickRayDir, matInvView);
		//vPickRayDir.Normalize();


		// face list
		for (int face = 0; face < m_Map.m_IndexList.size() / 3; face++)
		{
			v0 = m_Map.m_VertexList[m_Map.m_IndexList[face * 3 + 0]].p;
			v1 = m_Map.m_VertexList[m_Map.m_IndexList[face * 3 + 1]].p;
			v2 = m_Map.m_VertexList[m_Map.m_IndexList[face * 3 + 2]].p;

			vEnd = vPickRayOrigin + vPickRayDir * 100.0f;
			vNormal = (v1 - v0).Cross(v2 - v0);
			vNormal.Normalize();

			// 방법 1) 외적을 사용하는 방법
			//if (m_Picking.GetIntersection(vPickRayOrigin, vEnd, vNormal, v0, v1, v2))
			//{
			//	if (m_Picking.PointInPolygon(m_Picking.m_vInterSection, vNormal, v0, v1, v2))
			//	{
			//		list[0] = v0;
			//		list[1] = v1;
			//		list[2] = v2;
			//	}
			//}

			// 방법 2) UV 매개변수를 사용한 교점과 교점 포함 테스트를
			//         동시에 처리 하는 방법(평면 노말 필요X).
			if (m_Picking.IntersectTriangle(vPickRayOrigin, vPickRayDir, v0, v1, v2, t, u, v))
			{
				list[0] = v0;
				list[1] = v1;
				list[2] = v2;
			}

		}
	}



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
	}

	m_UserShape.Frame();
	m_UserShape.m_vPos.y = m_Map.GetHeightMap(m_UserShape.m_matWorld._41, m_UserShape.m_matWorld._43);

	m_pMainCamera->m_vCameraTarget = m_UserShape.m_vPos;
	m_pMainCamera->FrameFrustum(g_pImmediateContext);

	m_UserShape.m_matRotation = m_pMainCamera->m_matWorld;


	return true;
}

bool Sample::Render()
{

	if (m_Minimap.Begin(g_pImmediateContext))
	{
		m_Map.SetMatrix(NULL, &m_TopCamera.m_matView, &m_TopCamera.m_matProject);
		m_Map.Render(g_pImmediateContext);

		Matrix matWorld;
		matWorld._41 = m_TopCamera.m_vCameraPos.x;
		matWorld._42 = m_TopCamera.m_vCameraPos.y;
		matWorld._43 = m_TopCamera.m_vCameraPos.z;

		m_UserShape.SetMatrix(NULL, &m_TopCamera.m_matView, &m_TopCamera.m_matProject);
		m_UserShape.Render(g_pImmediateContext);

		m_BoxShape.SetMatrix(NULL, &m_TopCamera.m_matView, &m_TopCamera.m_matProject);
		m_BoxShape.Render(g_pImmediateContext);

		m_Minimap.End(g_pImmediateContext);
	}


	m_Map.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
	// m_Map.Render(g_pImmediateContext);

	m_UserShape.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
	m_UserShape.Render(g_pImmediateContext);

	m_Minimap.SetMatrix(NULL, NULL, NULL);
	m_Minimap.Render(g_pImmediateContext);

	m_QuadTree.Render(g_pImmediateContext);

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
	m_QuadTree.Release();
	m_Minimap.Release();
	m_Map.Release();
	m_UserShape.Release();
	m_BoxShape.Release();
	m_PlaneShape.Release();

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


