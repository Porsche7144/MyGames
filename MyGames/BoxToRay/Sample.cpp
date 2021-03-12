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

	m_HSphere.vCenter = Vector3(-10.0f, 0.0f, 0.0f);
	m_HSphere.fRadius = 5.0f;
	m_ModelCamera.m_matWorld = Matrix::Identity;

	if (!m_ShapeSphere.Create(g_pd3dDevice, L"VS.txt", L"PS.txt", L"../../Image/tileA.jpg"))
	{
		return false;
	}

	// 카메라 바꿔치는 부분.
	m_ModelCamera.CreateViewMatrix({ 0,10,-10 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_ModelCamera.CreateProjectionMatrix(1, 1000, HBASIS_PI / 4.0f, fAspect);
	m_ModelCamera.Init();
	// 프러스텀 생성
	m_ModelCamera.CreateFrustum(g_pd3dDevice, g_pImmediateContext);
	m_pMainCamera = &m_ModelCamera;


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
		m_Select.m_Picking.m_Ray.vDir.x = vec.x * matInvView._11 + vec.y * matInvView._21 + vec.z * matInvView._31;
		m_Select.m_Picking.m_Ray.vDir.y = vec.x * matInvView._12 + vec.y * matInvView._22 + vec.z * matInvView._32;
		m_Select.m_Picking.m_Ray.vDir.z = vec.x * matInvView._13 + vec.y * matInvView._23 + vec.z * matInvView._33;
		m_Select.m_Picking.m_Ray.vDir.Normalize();

		m_Select.m_Picking.m_Ray.vOrigin.x = matInvView._41;
		m_Select.m_Picking.m_Ray.vOrigin.y = matInvView._42;
		m_Select.m_Picking.m_Ray.vOrigin.z = matInvView._43;

		// 방법 2) 함수 사용 정점 변환
		//vPickRayOrigin = Vector3{ 0.0f, 0.0f, 0.0f };
		//vPickRayDir = v;
		//vPickRayOrigin = Vector3::Transform(vPickRayOrigin, matInvView);
		//vPickRayDir = Vector3::TransformNormal(vPickRayDir, matInvView);
		//vPickRayDir.Normalize();

		if (m_Select.IntersectRayToSphere(&m_HSphere, &m_Select.m_Picking.m_Ray))
		{
			MessageBox(0, _T("충돌"), _T("충돌"), MB_OK);
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

	m_pMainCamera->m_vCameraTarget = m_UserShape.m_vPos;
	m_pMainCamera->FrameFrustum(g_pImmediateContext);

	m_UserShape.m_matRotation = m_pMainCamera->m_matWorld;


	return true;
}

bool Sample::Render()
{
	Matrix matWorld, matScale;

	{
		Vector3 scale;
		scale.x = m_HSphere.fRadius;
		scale.y = m_HSphere.fRadius;
		scale.z = m_HSphere.fRadius;

		matWorld = matScale.CreateScale(scale);
		matWorld._41 = m_HSphere.vCenter.x;
		matWorld._42 = m_HSphere.vCenter.y;
		matWorld._43 = m_HSphere.vCenter.z;

		m_ShapeSphere.SetMatrix(&matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
		m_ShapeSphere.Render(g_pImmediateContext);
	}


	m_UserShape.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);

	m_LineShape.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
	m_LineShape.Draw(g_pImmediateContext, m_Select.m_Picking.m_Ray.vOrigin, 
					m_Select.m_Picking.m_Ray.vOrigin + m_Select.m_Picking.m_Ray.vDir * 100.0f);
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


