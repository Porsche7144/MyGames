#include "Sample.h"
#include "HTextureManager.h"

template<typename T>
struct arrayDelete
{
	void operator() (T const* p)
	{
		delete[] p;
	}
};

HRESULT Sample::InitWork()
{
	HRESULT hr = S_OK;

	m_pSplatting[0] = g_TextureMgr.Load(g_pd3dDevice, L"../../Image/data/map/woodfloor.bmp");
	m_pSplatting[1] = g_TextureMgr.Load(g_pd3dDevice, L"../../Image/data/map/seafloor.bmp");
	m_pSplatting[2] = g_TextureMgr.Load(g_pd3dDevice, L"../../Image/data/map/stone_wall.bmp");
	m_pSplatting[3] = g_TextureMgr.Load(g_pd3dDevice, L"../../Image/data/map/land.bmp");

	m_SelectData[0].fRadius = 100.0f;
	m_SelectData[0].iIndex = -1; // z
	m_SelectData[0].vPickPos = Vector3(0, 0, 0);
	m_SelectData[0].fTexHeight = 1024.0f;
	m_SelectData[0].fTexWidth = 1024.0;
	hr = CreateStructuredBuffer(g_pd3dDevice, sizeof(BufferType), 1, &m_SelectData[0], m_pSelectBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	hr = CreateBufferSRV(g_pd3dDevice, m_pSelectBuffer.Get(), m_pSelectBufferSRV.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	hr = CreateTextureUAV(g_pd3dDevice, m_iTextureSizeX, m_iTextureSizeY);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = CreateComputeShader(g_pd3dDevice, L"../../data/Shader/CS.txt", "CS", m_pCS.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	ID3D11ShaderResourceView* arraySRV[3] =
	{
		m_Map.m_pTexture->m_pTextureSRV,
		m_pAlphaTextureSRVCopy.Get(),
		m_pSelectBufferSRV.Get(),
	};

	RunComputeShader(g_pImmediateContext, m_pCS.Get(), arraySRV, m_pAlphaTextureUAV.GetAddressOf(), 3,
				     m_iTextureSizeX / 32, m_iTextureSizeY / 32, 1);

	g_pImmediateContext->CopyResource(m_pAlphaTextureCopy.Get(), m_pAlphaTexture.Get());


	return hr;
}

HRESULT Sample::CreateComputeShader(ID3D11Device* pDevice, LPCWSTR srcFile, LPCSTR FuncName, 
									ID3D11ComputeShader** ppShaderOut)
{
	HRESULT hr = S_OK;
	*ppShaderOut = LoadComputeShaderFile(pDevice, srcFile, nullptr, "CS");
	if (*ppShaderOut == nullptr)
	{
		return E_FAIL;
	}

	return hr;
}

void Sample::RunComputeShader(ID3D11DeviceContext* pContext, ID3D11ComputeShader* pCS, ID3D11ShaderResourceView** ppSRV,
							  ID3D11UnorderedAccessView** ppUAV, int iNumView, UINT X, UINT Y, UINT Z)
{

	pContext->CSSetUnorderedAccessViews(0, 1, m_pAlphaTextureUAV.GetAddressOf(), NULL);
	pContext->CSSetShaderResources(0, 3, ppSRV);
	pContext->CSSetShader(pCS, NULL, 0);

	pContext->Dispatch(X, Y, Z);  // 32 * 32 = 1024

	// set 후 초기화
	// 초기화 하지 않으면 초기세팅에서 영원히 바뀌지 않음.
	pContext->CSSetShader(NULL, NULL, 0);
	ID3D11UnorderedAccessView* pUAVNULL[1] = { NULL };
	pContext->CSSetUnorderedAccessViews(0, 1, pUAVNULL, NULL);
	ID3D11ShaderResourceView* pSRVNULL[3] = { NULL, NULL, NULL };
	pContext->CSSetShaderResources(0, 3, pSRVNULL);
	ID3D11Buffer* pCBNULL[1] = { NULL };
	pContext->CSSetConstantBuffers(0, 1, pCBNULL);

}

HRESULT Sample::CreateStructuredBuffer(ID3D11Device* pDevice, UINT uElementSize, UINT uCount,
										VOID* pInitData, ID3D11Buffer** ppBufOut)
{
	// 구조화 버퍼 생성
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS; // 필수
	bd.ByteWidth = sizeof(BufferType) * 1;
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // 구조화버퍼
	bd.StructureByteStride = sizeof(BufferType);

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &m_SelectData;

	HRESULT hr = pDevice->CreateBuffer(&bd, &initData, m_pSelectBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;

}

HRESULT Sample::CreateBufferSRV(ID3D11Device* pDevice, ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSRVOut)
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC desc;
	pBuffer->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));
	srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvd.BufferEx.FirstElement = 0;
	// CS에서 unored로 설정하기 위해선 반드시 포맷을 UNKNOWN으로 설정
	srvd.Format = DXGI_FORMAT_UNKNOWN;
	srvd.BufferEx.NumElements = desc.ByteWidth / desc.StructureByteStride;
	hr = pDevice->CreateShaderResourceView(pBuffer, &srvd, ppSRVOut);
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

HRESULT Sample::CreateTextureUAV(ID3D11Device* pDevice, int iWidth, int iHeight)
{
	HRESULT hr = S_OK;
	// shared_ptr에서 배열 해제시 delete를 처리할 구조체를 입력.
	std::shared_ptr<int> buf(new int[iWidth * iHeight], arrayDelete<int>());
	for (int i = 0; i < iHeight; i++)
	{
		for (int j = 0; j < iWidth; j++)
		{
			if ((i & 32) == (j & 32))
			{
				buf.get()[i* iWidth + j] = 0x00000000;
			}
			else
			{
				buf.get()[i* iWidth + j] = 0x00000000;
			}
		}
	}

	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = iWidth;
	td.Height = iHeight;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;  // 필수
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(subData));
	// D3D11_USAGE_STAGING  = CPU
	// D3D11_USAGE_DYNAMIC = CPU
	// UpdateSubresource = GPU
	// pSysMem = Init
	subData.pSysMem = (void*)buf.get();
	subData.SysMemPitch = iWidth * 4;
	// subData.SysMemSlicePicth = iWidth * iHeight * 4;  3D Texture
	
	hr = pDevice->CreateTexture2D(&td, &subData, m_pAlphaTexture.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavd;
	ZeroMemory(&uavd, sizeof(uavd));
	uavd.Format = td.Format;
	uavd.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavd.Texture2D.MipSlice = 0;
	hr = pDevice->CreateUnorderedAccessView(m_pAlphaTexture.Get(), &uavd, m_pAlphaTextureUAV.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));
	srvd.Format = td.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = 1;
	hr = pDevice->CreateShaderResourceView(m_pAlphaTexture.Get(), &srvd, m_pAlphaTextureSRV.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	// Copy
	hr = pDevice->CreateTexture2D(&td, &subData, m_pAlphaTextureCopy.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	hr = pDevice->CreateShaderResourceView(m_pAlphaTextureCopy.Get(), &srvd, m_pAlphaTextureSRVCopy.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;	

}