#include "Sample.h"
void Sample::CompilerCheck(ID3DBlob* ErrorMsg)
{
	C_STR szMsg = (char*)ErrorMsg->GetBufferPointer();
	T_STR szError = to_mw(szMsg);
	MessageBox(NULL, szError.c_str(), L"ERROR", MB_OK);
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

	if (!m_Box.Create(m_pd3dDevice, L"VS.txt", L"PS.txt", L"../../Image/KakaoTalk_20201201_210710448.jpg"))
	{
		return false;
	}
	if (!m_Plane.Create(m_pd3dDevice, L"VS.txt", L"PS.txt", L"../../Image/KakaoTalk_20201201_210710448.jpg"))
	{
		return false;
	}
	if (!m_Line.Create(m_pd3dDevice, L"VS.txt", L"PS.txt", L"../../Image/KakaoTalk_20201201_210710448.jpg"))
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
	m_matPlaneWorld = matScale * matRotation;
	m_matPlaneWorld._41 = 3.0f;

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