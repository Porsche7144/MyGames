#include "Sample.h"
void Sample::SetRasterState()
{
	HRESULT hr;
	// RASTERIZER 상태
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
	rd.FillMode = m_FillMode;
	rd.CullMode = m_CullMode;

	hr = m_pd3dDevice->CreateRasterizerState(&rd, &m_pRS);
	if (FAILED(hr))
	{
		return;
	}
}

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

	// Depth Texture 생성

	ID3D11Texture2D* pTexture = nullptr;
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	//UINT Width;
	//UINT Height;
	//UINT MipLevels;
	//UINT ArraySize;
	//DXGI_FORMAT Format;
	//DXGI_SAMPLE_DESC SampleDesc;
	//D3D11_USAGE Usage;
	//UINT BindFlags;
	//UINT CPUAccessFlags;
	//UINT MiscFlags;
	textureDesc.Width = g_rtClient.right;
	textureDesc.Height = g_rtClient.bottom;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hr = m_pd3dDevice->CreateTexture2D(&textureDesc, NULL, &pTexture);
	if (FAILED(hr))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = m_pd3dDevice->CreateDepthStencilView(pTexture , &dsvDesc, &m_pDSV);
	if (FAILED(hr))
	{
		return false;
	}
	pTexture->Release();

	D3D11_DEPTH_STENCIL_DESC dsdDesc;
	ZeroMemory(&dsdDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsdDesc.DepthEnable = TRUE;
	dsdDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsdDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = m_pd3dDevice->CreateDepthStencilState(&dsdDesc, &m_pDSS);
	if (FAILED(hr))
	{
		return false;
	}

	m_FillMode = D3D11_FILL_SOLID;
	m_CullMode = D3D11_CULL_NONE;
	SetRasterState();

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.BorderColor[0] = 1;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 1;
	samplerDesc.MinLOD = FLT_MIN;
	samplerDesc.MaxLOD = FLT_MAX;
	hr = m_pd3dDevice->CreateSamplerState(&samplerDesc, &m_pWrapLinear);


	// RASTERIZER 상태
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;
	hr = m_pd3dDevice->CreateRasterizerState(&rd, &m_pRS);
	if (FAILED(hr))
	{
		return false;
	}

	ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.CullMode = D3D11_CULL_BACK;
	hr = m_pd3dDevice->CreateRasterizerState(&rd, &m_pRS);
	if (FAILED(hr))
	{
		return false;
	}

	if (!m_Box.Create(m_pd3dDevice, L"VS.txt", L"PS.txt", L"../../Image/KakaoTalk_20201201_210710448.jpg"))
	{
		return false;
	}
	if (!m_Plane.Create(m_pd3dDevice, L"VS.txt", L"PS.txt", L"../../Image/KakaoTalk_20201201_210710448.jpg"))
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
		m_vCameraPos.y += 10.0f * g_fSecondPerFrame;
	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_vCameraPos.y -= 10.0f * g_fSecondPerFrame;
	}
	HVector3 u = { 0,1,0 };
	m_matView.CreateViewLook(m_vCameraPos, m_vCameraTarget, u);

	if (g_Input.GetKey('1') == KEY_PUSH)
	{
		m_FillMode = D3D11_FILL_WIREFRAME;
		SetRasterState();
	}

	if (g_Input.GetKey('2') == KEY_PUSH)
	{
		m_FillMode = D3D11_FILL_SOLID;
		SetRasterState();
	}

	if (g_Input.GetKey('3') == KEY_PUSH)
	{
		m_CullMode = D3D11_CULL_BACK;
		SetRasterState();
	}

	if (g_Input.GetKey('4') == KEY_PUSH)
	{
		m_CullMode = D3D11_CULL_FRONT;
		SetRasterState();
	}

	return true;
}

bool Sample::PreRender()
{
	HCore::PreRender();
	ID3D11RenderTargetView* pNullRTV = NULL;
	m_pd3dContext->OMSetRenderTargets(1, &pNullRTV, NULL);
	m_pd3dContext->OMSetRenderTargets(1, &m_pRednerTargetView, m_pDSV);
	m_pd3dContext->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	return true;
}

bool Sample::Render()
{
	m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dContext->RSSetState(m_pRS);
	m_pd3dContext->PSSetSamplers(0, 1, &m_pWrapLinear);
	m_pd3dContext->OMSetDepthStencilState(m_pDSS, 0);

	m_Box.SetMatrix(&m_matBoxWorld, &m_matView, &m_matProject);
	m_Box.Render(m_pd3dContext);

	m_Plane.SetMatrix(&m_matPlaneWorld, &m_matView, &m_matProject);
	m_Plane.Render(m_pd3dContext);

	return true;
}

bool Sample::Release()
{
	m_pDSS->Release();
	m_pDSV->Release();

	m_pWrapLinear->Release();

	m_pRS->Release();
	if (m_pRSSSolidBack != nullptr)
	{
		m_pRSSSolidBack->Release();
	}
	if (m_pRSWireBack != nullptr)
	{
		m_pRSWireBack->Release();
	}
	m_Box.Release();

	return true;
}