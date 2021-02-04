#include "Sample.h"

void Sample::CompilerCheck(ID3DBlob* ErrorMsg)
{
	C_STR szMsg = (char*)ErrorMsg->GetBufferPointer();
	T_STR szError = to_mw(szMsg);
	MessageBox(NULL, szError.c_str(), L"ERROR", MB_OK);
}

bool Sample::Init()
{
	// Vertex버퍼
	// P_VERTEX pos = { HVertex3(-0.3f, 0.3f, 0.5f), HVertex3(0,0,0) };

	m_VertexList.resize(6);
	m_VertexList[0] = { HVertex3(-0.3f, 0.3f, 0.5f), HVertex3(0,0,0) };
	m_VertexList[1] = { HVertex3(0.3f, 0.3f, 0.5f), HVertex3(0,0,0) };
	m_VertexList[2] = { HVertex3(0.0f, 0.0f, 0.5f), HVertex3(0,0,0) };
	m_VertexList[3] = { HVertex3(0.0f, 0.0f, 0.5f), HVertex3(0,0,0) };
	m_VertexList[4] = { HVertex3(0.3f, -0.3f, 0.5f), HVertex3(0,0,0) };
	m_VertexList[5] = { HVertex3(-0.3f, -0.3f, 0.5f), HVertex3(0,0,0) };

	D3D11_BUFFER_DESC bd;
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

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_VertexList.at(0);
	HRESULT hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer);
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
	hr = D3DCompileFromFile(L"VS.txt", NULL, NULL, "PS", "ps_5_0", 0, 0, &pPSObj, &pErrorMsg);
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

bool Sample::Render()
{
	UINT iStride = sizeof(P_VERTEX);
	UINT iOffset = 0;
	m_pd3dContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &iStride, &iOffset);
	m_pd3dContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pd3dContext->IASetInputLayout(m_pInputLayout);
	m_pd3dContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pd3dContext->PSSetShader(m_pPixelShader, NULL, 0);
	m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// Vertex Draw
	m_pd3dContext->Draw(m_VertexList.size(), 0);

	// Index Draw
	// m_pd3dContext->DrawIndexed(m_IndexList.size(), 0, 0);

	return true;
}

bool Sample::Release()
{
	m_pVertexBuffer->Release();
	m_pIndexBuffer->Release();
	m_pInputLayout->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();

	return true;
}