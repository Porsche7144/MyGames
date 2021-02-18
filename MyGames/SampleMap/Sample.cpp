#include "Sample.h"

LRESULT	 Sample::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pMainCamera == nullptr) return -1;
	m_pMainCamera->WndProc(hWnd, message, wParam, lParam);

	return -1;
}

Matrix* TD3DXMatrixShadow(Matrix *pout,
	Vector4 *plight,
	Vector4 *pplane)
{
	Vector4 Nplane;
	FLOAT dot;
	Nplane.Normalize();
	//D3DXPlaneNormalize(&Nplane, pplane);
	dot = Nplane.Dot(*plight);
	pout->m[0][0] = dot - Nplane.x * plight->x;
	pout->m[0][1] = -Nplane.x * plight->y;
	pout->m[0][2] = -Nplane.x * plight->z;
	pout->m[0][3] = -Nplane.x * plight->w;
	pout->m[1][0] = -Nplane.y * plight->x;
	pout->m[1][1] = dot - Nplane.y * plight->y;
	pout->m[1][2] = -Nplane.y * plight->z;
	pout->m[1][3] = -Nplane.y * plight->w;
	pout->m[2][0] = -Nplane.z * plight->x;
	pout->m[2][1] = -Nplane.z * plight->y;
	pout->m[2][2] = dot - Nplane.z * plight->z;
	pout->m[2][3] = -Nplane.z * plight->w;
	pout->m[3][0] = -Nplane.w * plight->x;
	pout->m[3][1] = -Nplane.w * plight->y;
	pout->m[3][2] = -Nplane.w * plight->z;
	pout->m[3][3] = dot - Nplane.w * plight->w;
	return pout;
}

Matrix Sample::CreateMatrixShadow(
	Vector4* pPlane,
	Vector4* pLight)
{
	Matrix mat;
	Vector4 plane, light;
	pPlane->Normalize();
	plane.x = pPlane->x * -1.0f;
	plane.y = pPlane->y * -1.0f;
	plane.z = pPlane->z * -1.0f;
	plane.w = pPlane->w * -1.0f;
	light = *pLight;// * -1.0f;
	float D = -(plane.Dot(light));
	mat._11 = plane.x * light.x + D;	mat._12 = plane.x * light.y;	mat._13 = plane.x * light.z;	mat._14 = plane.x * light.w;
	mat._21 = plane.y * light.x;	mat._22 = plane.y * light.y + D;	mat._23 = plane.y * light.z;	mat._24 = plane.y * light.w;
	mat._31 = plane.z * light.x;	mat._32 = plane.z * light.y;	mat._33 = plane.z * light.z + D;	mat._34 = plane.z * light.w;
	mat._41 = plane.w * light.x;	mat._42 = plane.w * light.y;	mat._43 = plane.w * light.z;	mat._44 = plane.w * light.w + D;
	return mat;
}

bool Sample::Init()
{
	HRESULT hr;
	m_vDirValue = { 0,0,0,0 };

	Matrix matScale, matRotation;
	matScale = Matrix::CreateScale(100, 100, 0);
	matRotation = Matrix::CreateRotationX(HBASIS_PI * 0.5f);
	m_matPlaneWorld = matScale * matRotation;

	if (!m_BoxShape.Create(m_pd3dDevice, L"VS.txt", L"PS.txt", L"../../Image/KakaoTalk_20201201_210710448.jpg"))
	{
		return false;
	}
	if (!m_PlaneShape.Create(m_pd3dDevice, L"VS.txt", L"PS.txt", L"../../Image/KakaoTalk_20201201_210710448.jpg"))
	{
		return false;
	}
	if (!m_LineShape.Create(m_pd3dDevice, L"VS.txt", L"PS.txt", L"../../KakaoTalk_20201201_210710448.jpg"))
	{
		return false;
	}

	// 카메라 바꿔치는 부분.
	m_ModelCamera.CreateViewMatrix({ 0,10,-1 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_ModelCamera.CreateProjectionMatrix(1, 1000, HBASIS_PI / 4.0f, fAspect);
	m_ModelCamera.Init();
	m_pMainCamera = &m_ModelCamera;
	HMapDesc desc;
	desc.iNumCols = 257;
	desc.iNumRows = 257;
	desc.fCellDistance = 1;
	desc.szTextFile = L"../../Image/KakaoTalk_20201201_210710448.jpg";
	desc.szVS = L"VS.txt";
	desc.szPS = L"PS.txt";
	m_Map.CreateMap(m_pd3dDevice, desc);

	return true;
}

bool Sample::Frame()
{

	if (g_Input.GetKey('1') == KEY_PUSH)
	{
		HDxState::m_FillMode = D3D11_FILL_WIREFRAME;
		HDxState::SetRasterState(m_pd3dDevice);
	}

	if (g_Input.GetKey('2') == KEY_PUSH)
	{
		HDxState::m_FillMode = D3D11_FILL_SOLID;
		HDxState::SetRasterState(m_pd3dDevice);
	}

	if (g_Input.GetKey('3') == KEY_PUSH)
	{
		HDxState::m_CullMode = D3D11_CULL_BACK;
		HDxState::SetRasterState(m_pd3dDevice);
	}

	if (g_Input.GetKey('4') == KEY_PUSH)
	{
		HDxState::m_CullMode = D3D11_CULL_FRONT;
		HDxState::SetRasterState(m_pd3dDevice);
	}

	if (g_Input.GetKey('W') == KEY_HOLD)
	{
		m_BoxShape.FrontMovement(1.0f);
	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_BoxShape.FrontMovement(-1.0f);
	}
	if (g_Input.GetKey('A') == KEY_HOLD)
	{
		m_BoxShape.RightMovement(-1.0f);
	}
	if (g_Input.GetKey('D') == KEY_HOLD)
	{
		m_BoxShape.RightMovement(1.0f);
	}
	if (g_Input.GetKey('Q') == KEY_HOLD)
	{
		m_BoxShape.UpMovement(1.0f);
	}
	if (g_Input.GetKey('E') == KEY_HOLD)
	{
		m_BoxShape.UpMovement(-1.0f);
	}

	m_BoxShape.Frame();
	m_pMainCamera->m_vCameraTarget = m_BoxShape.m_vPos;

	return true;
}

bool Sample::Render()
{
	m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dContext->RSSetState(HDxState::m_pRS);
	m_pd3dContext->PSSetSamplers(0, 1, &HDxState::m_pWrapLinear);
	m_pd3dContext->OMSetDepthStencilState(HDxState::m_pDSS, 0);

	m_BoxShape.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
	m_BoxShape.Render(m_pd3dContext);

	//Matrix matShadow;
	//Vector4 PLANE = Vector4(0, 1, 0, -0.1f);
	//Vector3 vLightDir = Vector3(-10, 10, 0);
	//// Vector4 LIGHT = Vector4(-10, 10, 0, 1);
	//// matShadow = CreateMatrixShadow(&PLANE, &LIGHT);
	//matShadow = Matrix::CreateShadow(vLightDir, PLANE);

	//matShadow = m_matBoxWorld * matShadow;
	//m_BoxShape.SetMatrix(&matShadow, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
	//m_BoxShape.Render(m_pd3dContext);

	m_Map.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
	m_Map.Render(m_pd3dContext);

	m_LineShape.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
	m_LineShape.Draw(m_pd3dContext, Vector3(0, 0, 0), Vector3(100, 0, 0), Vector4(1, 0, 0, 1));
	m_LineShape.Draw(m_pd3dContext, Vector3(0, 0, 0), Vector3(0, 100, 0), Vector4(0, 1, 0, 1));
	m_LineShape.Draw(m_pd3dContext, Vector3(0, 0, 0), Vector3(0, 0, 100), Vector4(0, 0, 1, 1));

	return true;
}

bool Sample::Release()
{
	m_BoxShape.Release();
	m_PlaneShape.Release();
	m_LineShape.Release();

	return true;
}


bool HBoxUser::PreFrame()
{
	m_matWorld._41 = m_vPos.x;
	m_matWorld._42 = m_vPos.y;
	m_matWorld._43 = m_vPos.z;
	UpdateVector();

	return true;
}

void HBoxUser::UpdateVector()
{
	m_vLook.x = m_matWorld._31;
	m_vLook.y = m_matWorld._32;
	m_vLook.z = m_matWorld._33;
	m_vUp.x = m_matWorld._21;
	m_vUp.y = m_matWorld._22;
	m_vUp.z = m_matWorld._23;
	m_vRight.x = m_matWorld._11;
	m_vRight.y = m_matWorld._12;
	m_vRight.z = m_matWorld._13;

	m_vLook.Normalize();
	m_vUp.Normalize();
	m_vRight.Normalize();
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


