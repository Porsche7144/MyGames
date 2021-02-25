#include "HDxObject.h"
#include "HTextureManager.h"

void HDxObject::CompilerCheck(ID3DBlob* ErrorMsg)
{
	C_STR szMsg = (char*)ErrorMsg->GetBufferPointer();
	T_STR szError = to_mw(szMsg);
	MessageBox(NULL, szError.c_str(), L"ERROR", MB_OK);
}

bool HDxObject::Create(ID3D11Device * pDevice, T_STR vs, T_STR ps, T_STR texture)
{
	m_pd3dDevice = pDevice;

	CreateVertexData();
	CreateVertexBuffer();
	CreateConstantBuffer();
	CreateIndexData();
	CreateIndexBuffer();
	LoadShader(vs, ps);
	CreateInputLayOut();
	LoadTexture(texture);

	return true;
}

bool HDxObject::CreateVertexBuffer()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	/*UINT ByteWidth;
	D3D11_USAGE Usage;
	UINT BindFlags;
	UINT CPUAccessFlags;
	UINT MiscFlags;
	UINT StructureByteStride;*/
	bd.ByteWidth = sizeof(PNCT_VERTEX) * m_VertexList.size();
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

	return true;
}

bool HDxObject::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	// 인덱스 버퍼로 바인딩
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_IndexList.at(0);
	HRESULT hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pIndexBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool HDxObject::CreateConstantBuffer()
{
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
	HRESULT hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_ConstantBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool HDxObject::CreateVertexData()
{
	return true;
}

bool HDxObject::CreateIndexData()
{
	return true;
}

bool HDxObject::CreateInputLayOut()
{
	// LayOut
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iNumElement = sizeof(layout) / sizeof(layout[0]);
	HRESULT hr = m_pd3dDevice->CreateInputLayout(layout, iNumElement, pVSObj->GetBufferPointer(),
		pVSObj->GetBufferSize(), &m_pInputLayout);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool HDxObject::Update(ID3D11DeviceContext* pd3dContext)
{

	D3D11_MAPPED_SUBRESOURCE ms;
	HRESULT hr = pd3dContext->Map(m_ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	if (SUCCEEDED(hr))
	{
		HDataCB* pData = (HDataCB*)ms.pData;
		pData->matWorld = m_matWorld.Transpose();
		pData->matView = m_matView.Transpose();
		pData->matProject = m_matProject.Transpose();

		pData->vColor[0] = cosf(g_fGameTimer);
		pData->vColor[1] = sinf(g_fGameTimer);
		pData->vColor[2] = 1.0f - cosf(g_fGameTimer);
		pData->vColor[3] = 1;
		pData->vTime[0] = cosf(g_fGameTimer) * 0.5f + 0.5f;
		pData->vTime[1] = g_fGameTimer;
		// map 사용 후 unmap 쓰레드. mutex와 같음
		pd3dContext->Unmap(m_ConstantBuffer, 0);
	}

	return true;
}

bool HDxObject::LoadShader(T_STR vs, T_STR ps)
{
	// 쉐이더
	ID3DBlob* pPSObj;
	ID3DBlob* pErrorMsg;

	// Vertex 쉐이더
	HRESULT hr = D3DCompileFromFile(vs.c_str(), NULL, NULL, m_szVertexShader.c_str(), "vs_5_0", 0, 0, &pVSObj, &pErrorMsg);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsg);
		return false;
	}
	hr = m_pd3dDevice->CreateVertexShader(pVSObj->GetBufferPointer(), pVSObj->GetBufferSize(), NULL,
		&m_pVertexShader);
	if (FAILED(hr))
	{
		return false;
	}
	// Pixel 쉐이더
	hr = D3DCompileFromFile(ps.c_str(), NULL, NULL, m_szPixelShader.c_str(), "ps_5_0", 0, 0, &pPSObj, &pErrorMsg);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsg);
		return false;
	}
	hr = m_pd3dDevice->CreatePixelShader(pPSObj->GetBufferPointer(), pPSObj->GetBufferSize(), NULL,
		&m_pPixelShader);
	if (FAILED(hr))
	{
		return false;
	}

	if (pPSObj)
	{
		pPSObj->Release();
	}

	return true;
}

bool HDxObject::LoadTexture(T_STR texture)
{
	// 텍스쳐 로드
	m_pTexture = g_TextureMgr.Load(m_pd3dDevice, texture.c_str());
	if (m_pTexture == nullptr) return false;

	return true;
}

bool HDxObject::SetMatrix(Matrix * pWorld, Matrix * pView, Matrix * pProj)
{
	if (pWorld != nullptr)
	{
		m_matWorld = *pWorld;
	}
	else
	{
		m_matWorld = m_matScale * m_matRotation;
		m_matWorld._41 = m_vPos.x;
		m_matWorld._42 = m_vPos.y;
		m_matWorld._43 = m_vPos.z;
	}
	if (pView != nullptr)
	{
		m_matView = *pView;
	}
	if (pProj != nullptr)
	{
		m_matProject = *pProj;
	}

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

	return true;
}

bool HDxObject::Init()
{
	m_matWorld = Matrix::Identity;
	m_matView = Matrix::Identity;
	m_matProject = Matrix::Identity;

	return true;
}

bool HDxObject::Frame()
{
	return true;
}

bool HDxObject::PreRender(ID3D11DeviceContext * pContext)
{
	return true;
}

bool HDxObject::Render(ID3D11DeviceContext* pContext)
{
	Update(pContext);
	PreRender(pContext);
	UINT iStride = sizeof(PNCT_VERTEX);
	UINT iOffset = 0;
	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &iStride, &iOffset);
	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pContext->IASetInputLayout(m_pInputLayout);

	pContext->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	pContext->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);

	pContext->VSSetShader(m_pVertexShader, NULL, 0);
	pContext->PSSetShader(m_pPixelShader, NULL, 0);
	pContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iTopology);
	if (m_pTexture != nullptr)
	{
		pContext->PSSetShaderResources(0, 1, &m_pTexture->m_pTextureSRV);
	}
	// Vertex Draw
	// m_pd3dContext->Draw(m_VertexList.size(), 0);

	PostRender(pContext);

	return true;
}

bool HDxObject::PostRender(ID3D11DeviceContext * pContext)
{
	pContext->DrawIndexed(m_IndexList.size(), 0, 0);
	return true;
}

bool HDxObject::Release()
{
	m_ConstantBuffer->Release();
	m_pVertexBuffer->Release();
	m_pIndexBuffer->Release();
	m_pInputLayout->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();

	return true;
}

HDxObject::HDxObject()
{
	m_szVertexShader = "VS";
	m_szPixelShader = "PS";
	m_iTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

HDxObject::~HDxObject()
{
}
