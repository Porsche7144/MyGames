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

	m_matWorld.Identity();
	HVector3 p = m_vCameraPos;
	HVector3 t = m_vCameraTarget;
	HVector3 u = { 0,0,0 };

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

	// 텍스쳐 로드
	ID3D11Resource* texture;
	hr = DirectX::CreateWICTextureFromFile(m_pd3dDevice, L"../../Image/KakaoTalk_20201201_210710448.jpg"
											, NULL, &m_pTextureSRV);

	m_FillMode = D3D11_FILL_SOLID;
	m_CullMode = D3D11_CULL_BACK;
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

	hr;
	// RASTERIZER 상태
	rd;
	ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.CullMode = D3D11_CULL_BACK;
	hr = m_pd3dDevice->CreateRasterizerState(&rd, &m_pRS);
	if (FAILED(hr))
	{
		return false;
	}


	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(HDataCB);
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// Constant 버퍼로 바인딩
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_cbData;
	hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_ConstantBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	// Vertex버퍼
	// P_VERTEX pos = { HVertex3(-0.3f, 0.3f, 0.5f), HVertex3(0,0,0) };

	m_VertexList.resize(4);
	m_VertexList[0] = { HVector3(-1.0f, 1.0f, 0.5f), 
						HVector3(0,0,0),
						HVector4(1,0,0,1),
						HVector2(0,0)};

	m_VertexList[1] = { HVector3(1.0f, 1.0f, 0.5f),
						HVector3(0,0,0),
						HVector4(0,1,0,1),
						HVector2(2,0) };

	m_VertexList[2] = { HVector3(-1.0f, -1.0f, 0.5f),
						HVector3(0,0,0),
						HVector4(0,0,1,1),
						HVector2(0,2)};

	m_VertexList[3] = { HVector3(1.0f, -1.0f, 0.5f),
						HVector3(0,0,0),
						HVector4(1,1,1,1),
						HVector2(2,2)};

	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	/*UINT ByteWidth;
	D3D11_USAGE Usage;
	UINT BindFlags;
	UINT CPUAccessFlags;
	UINT MiscFlags;
	UINT StructureByteStride;*/
	bd.ByteWidth = sizeof(P_VERTEX) * m_VertexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	// Vertex 버퍼로 바인딩
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_VertexList.at(0);
	hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	// Index 버퍼
	/*DWORD p[4];
	p[0] = pos.pos.x;
	p[1] = pos.pos.y;
	p[2] = pos.pos.z;
	p[3] = -0.3f;*/

	m_IndexList.resize(6);
	m_IndexList[0] = 0;
	m_IndexList[1] = 1;
	m_IndexList[2] = 2;
	m_IndexList[3] = 2;
	m_IndexList[4] = 1;
	m_IndexList[5] = 3;

	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(P_VERTEX) * m_IndexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	// 인덱스 버퍼로 바인딩
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_IndexList.at(0);
	hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pIndexBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	// 쉐이더
	ID3DBlob* pVSObj;
	ID3DBlob* pPSObj;
	ID3DBlob* pErrorMsg;

	// Vertex 쉐이더
	hr = D3DCompileFromFile(L"VS.txt", NULL, NULL, "VS", "vs_5_0", 0, 0, &pVSObj, &pErrorMsg);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsg);
		return false;
	}
	hr = m_pd3dDevice->CreateVertexShader(pVSObj->GetBufferPointer(), pVSObj->GetBufferSize(), NULL,
		&m_pVertexShader);

	// Pixel 쉐이더
	hr = D3DCompileFromFile(L"PS.txt", NULL, NULL, "PS", "ps_5_0", 0, 0, &pPSObj, &pErrorMsg);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsg);
		return false;
	}
	hr = m_pd3dDevice->CreatePixelShader(pPSObj->GetBufferPointer(), pPSObj->GetBufferSize(), NULL, 
		&m_pPixelShader);

	// LayOut
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iNumElement = sizeof(layout) / sizeof(layout[0]);
	hr = m_pd3dDevice->CreateInputLayout(layout, iNumElement, pVSObj->GetBufferPointer(),
		pVSObj->GetBufferSize(), &m_pInputLayout);

	return true;
}

bool Sample::Frame()
{
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

	D3D11_MAPPED_SUBRESOURCE ms;
	HRESULT hr = m_pd3dContext->Map(m_ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	if (SUCCEEDED(hr))
	{
		HDataCB* pData = (HDataCB*)ms.pData;
		pData->vColor[0] = cosf(g_fGameTimer);
		pData->vColor[1] = sinf(g_fGameTimer);
		pData->vColor[2] = 1.0f - cosf(g_fGameTimer);
		pData->vColor[3] = 1;
		pData->vTime[0] = cosf(g_fGameTimer) * 0.5f + 0.5f;
		pData->vTime[1] = g_fGameTimer;
		// map 사용 후 unmap 쓰레드. mutex와 같음
		m_pd3dContext->Unmap(m_ConstantBuffer, 0);
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
	UINT iStride = sizeof(P_VERTEX);
	UINT iOffset = 0;
	m_pd3dContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &iStride, &iOffset);
	m_pd3dContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pd3dContext->IASetInputLayout(m_pInputLayout);

	m_pd3dContext->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	m_pd3dContext->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);

	m_pd3dContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pd3dContext->PSSetShader(m_pPixelShader, NULL, 0);
	m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pd3dContext->RSSetState(m_pRS);
	
	m_pd3dContext->PSSetShaderResources(0, 1, &m_pTextureSRV);
	m_pd3dContext->PSSetSamplers(0, 1, &m_pWrapLinear);

	m_pd3dContext->OMSetDepthStencilState(m_pDSS, 0);
	// Vertex Draw
	// m_pd3dContext->Draw(m_VertexList.size(), 0);

	// Index Draw
	m_pd3dContext->DrawIndexed(m_IndexList.size(), 0, 0);

	return true;
}

bool Sample::Release()
{
	m_pDSS->Release();
	m_pDSV->Release();

	m_pWrapLinear->Release();
	m_pTextureSRV->Release();

	m_pRS->Release();
	if (m_pRSSSolidBack != nullptr)
	{
		m_pRSSSolidBack->Release();
	}
	if (m_pRSWireBack != nullptr)
	{
		m_pRSWireBack->Release();
	}
	m_ConstantBuffer->Release();

	m_pVertexBuffer->Release();
	m_pIndexBuffer->Release();
	m_pInputLayout->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();

	return true;
}