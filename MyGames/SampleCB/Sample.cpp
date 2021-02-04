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

	m_FillMode = D3D11_FILL_SOLID;
	m_CullMode = D3D11_CULL_BACK;
	SetRasterState();

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

	m_VertexList.resize(6);
	m_VertexList[0] = { HVertex3(-0.3f, 0.3f, 0.5f), HVertex3(0,0,0) };
	m_VertexList[1] = { HVertex3(0.3f, 0.3f, 0.5f), HVertex3(0,0,0) };
	m_VertexList[2] = { HVertex3(0.0f, 0.0f, 0.5f), HVertex3(0,0,0) };
	m_VertexList[3] = { HVertex3(0.0f, 0.0f, 0.5f), HVertex3(0,0,0) };
	m_VertexList[4] = { HVertex3(0.3f, -0.3f, 0.5f), HVertex3(0,0,0) };
	m_VertexList[5] = { HVertex3(-0.3f, -0.3f, 0.5f), HVertex3(0,0,0) };

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
		{"POSITION", 0 , DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	UINT iNumElement = sizeof(layout) / sizeof(layout[0]);
	hr = m_pd3dDevice->CreateInputLayout(layout, iNumElement, pVSObj->GetBufferPointer(),
		pVSObj->GetBufferSize(), &m_pInputLayout);

	return true;
}

bool Sample::Frame()
{
	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		m_FillMode = D3D11_FILL_WIREFRAME;
		SetRasterState();
	}

	if (g_Input.GetKey('9') == KEY_PUSH)
	{
		m_FillMode = D3D11_FILL_SOLID;
		SetRasterState();
	}

	if (g_Input.GetKey('8') == KEY_PUSH)
	{
		m_CullMode = D3D11_CULL_BACK;
		SetRasterState();
	}

	if (g_Input.GetKey('7') == KEY_PUSH)
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
		// map 사용 후 unmap 쓰레드 mutex와 같음
		m_pd3dContext->Unmap(m_ConstantBuffer, 0);
	}

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
	
	// Vertex Draw
	m_pd3dContext->Draw(m_VertexList.size(), 0);

	// Index Draw
	// m_pd3dContext->DrawIndexed(m_IndexList.size(), 0, 0);

	return true;
}

bool Sample::Release()
{
	m_pRS->Release();
	m_pRSSSolidBack->Release();
	m_pRSWireBack->Release();
	m_ConstantBuffer->Release();

	m_pVertexBuffer->Release();
	m_pIndexBuffer->Release();
	m_pInputLayout->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();

	return true;
}