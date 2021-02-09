#include "Sample.h"

HMatrix* TD3DXMatrixShadow(HMatrix *pout,
	HVector4 *plight,
	HVector4 *pplane)
{
	HVector4 Nplane;
	FLOAT dot;
	Nplane = Nplane.Normal();
	//D3DXPlaneNormalize(&Nplane, pplane);
	dot = Nplane | *plight;
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

HMatrix Sample::CreateMatrixShadow(
	HVector4* pPlane,
	HVector4* pLight)
{
	HMatrix mat;
	HVector4 plane, light;
	pPlane->Normal();
	plane.x = pPlane->x * -1.0f;
	plane.y = pPlane->y * -1.0f;
	plane.z = pPlane->z * -1.0f;
	plane.w = pPlane->w * -1.0f;
	light = *pLight;// * -1.0f;
	float D = -(plane | light);
	mat._11 = plane.x * light.x + D;	mat._12 = plane.x * light.y;	mat._13 = plane.x * light.z;	mat._14 = plane.x * light.w;
	mat._21 = plane.y * light.x;	mat._22 = plane.y * light.y + D;	mat._23 = plane.y * light.z;	mat._24 = plane.y * light.w;
	mat._31 = plane.z * light.x;	mat._32 = plane.z * light.y;	mat._33 = plane.z * light.z + D;	mat._34 = plane.z * light.w;
	mat._41 = plane.w * light.x;	mat._42 = plane.w * light.y;	mat._43 = plane.w * light.z;	mat._44 = plane.w * light.w + D;
	return mat;
}

bool Sample::Init()
{
	HRESULT hr;
	m_vCameraPos = { 10,10,-10 };
	m_vCameraTarget = { 0,0,0 };

	HVector3 p = m_vCameraPos;
	HVector3 t = m_vCameraTarget;
	HVector3 u = { 0,1,0 };
	m_matView.CreateViewLook(p, t, u);
	float fN = 1;
	float fF = 1000;
	float fFov = HBASIS_PI / 4.0f;
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_matProject.PerspectiveFovLH(fN, fF, fFov, fAspect);

	HMatrix matScale, matRotation;
	matScale.Scale(100, 100, 0);
	matRotation.XRotate(HBASIS_PI * 0.5f);
	m_matPlaneWorld = matScale * matRotation;

	if (!m_Box.Create(m_pd3dDevice, L"VS.txt", L"PS.txt", L"../../Image/tileA.jpg"))
	{
		return false;
	}
	if (!m_Plane.Create(m_pd3dDevice, L"VS.txt", L"PS.txt", L"../../Image/tileA.jpg"))
	{
		return false;
	}
	if (!m_Line.Create(m_pd3dDevice, L"VS.txt", L"PS.txt", L"../../tileA.jpg"))
	{
		return false;
	}

	return true;
}

bool Sample::Frame()
{
	HMatrix matScale;
	HMatrix matRotation;
	matScale.Scale(1, 1, 1);
	matRotation.YRotate(g_fGameTimer);
	m_matBoxWorld = matScale * matRotation;
	m_matBoxWorld._42 = 3.0f;

	if (g_Input.GetKey('W') == KEY_HOLD)
	{
		m_vCameraPos.z += 10.0f * g_fSecondPerFrame;
	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_vCameraPos.z -= 10.0f * g_fSecondPerFrame;
	}
	HVector3 u = { 0,1,0 };
	m_matView.CreateViewLook(m_vCameraPos, m_vCameraTarget, u);

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

	return true;
}

bool Sample::Render()
{
	m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dContext->RSSetState(HDxState::m_pRS);
	m_pd3dContext->PSSetSamplers(0, 1, &HDxState::m_pWrapLinear);
	m_pd3dContext->OMSetDepthStencilState(HDxState::m_pDSS, 0);

	m_Box.SetMatrix(&m_matBoxWorld, &m_matView, &m_matProject);
	m_Box.Render(m_pd3dContext);

	HMatrix matShadow;
	HVector4 PLANE = HVector4(0, 1, 0, -0.1f);
	HVector4 LIGHT = HVector4(-10, 10, 0, 1);
	matShadow = CreateMatrixShadow(&PLANE, &LIGHT);
	matShadow = m_matBoxWorld * matShadow;
	m_Box.SetMatrix(&matShadow, &m_matView, &m_matProject);
	m_Box.Render(m_pd3dContext);

	m_Plane.SetMatrix(&m_matPlaneWorld, &m_matView, &m_matProject);
	m_Plane.Render(m_pd3dContext);

	m_Line.SetMatrix(NULL, &m_matView, &m_matProject);
	m_Line.Draw(m_pd3dContext, HVector3(0, 0, 0), HVector3(100, 0, 0), HVector4(1, 0, 0, 1));
	m_Line.Draw(m_pd3dContext, HVector3(0, 0, 0), HVector3(0, 100, 0), HVector4(0, 1, 0, 1));
	m_Line.Draw(m_pd3dContext, HVector3(0, 0, 0), HVector3(0, 0, 100), HVector4(0, 0, 1, 1));

	return true;
}

bool Sample::Release()
{
	m_Box.Release();
	m_Plane.Release();
	m_Line.Release();

	return true;
}