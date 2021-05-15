#include "HTextureMap.h"

HRESULT HTextureMap::CreateStagingTexture2D(HMap* map, ID3D11DeviceContext* pContext)
{

	HRESULT hr;
	pStaging = nullptr;
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	textureDesc.Width = 512;
	textureDesc.Height = 512;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_STAGING;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	textureDesc.BindFlags = 0;
	textureDesc.MiscFlags = 0;
	hr = g_pd3dDevice->CreateTexture2D(&textureDesc, NULL, &pStaging);
	if (FAILED(hr))
	{
		return hr;
	}

	WriteTextureData(map, pContext);

	return hr;
}

HRESULT	HTextureMap::SetStaging(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	// m_ColorMiniMapList;

	HRESULT hr;
	pTexture = nullptr;
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	textureDesc.Width = 512;
	textureDesc.Height = 512;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	hr = pDevice->CreateTexture2D(&textureDesc, NULL, &pTexture);


	textureDesc.Usage = D3D11_USAGE_STAGING;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	textureDesc.BindFlags = 0;
	textureDesc.MiscFlags = 0;
	hr = pDevice->CreateTexture2D(&textureDesc, NULL, &pStaging);
	if (FAILED(hr))
	{
		return hr;
	}

	pContext->CopyResource(pTexture, pStaging);

	return hr;
}

HRESULT	HTextureMap::SetRenderTargetView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	// m_ColorMiniMapList;

	HRESULT hr;
	pTexture = nullptr;
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	textureDesc.Width = 512;
	textureDesc.Height = 512;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	hr = pDevice->CreateTexture2D(&textureDesc, NULL, &pTexture);


	textureDesc.Usage = D3D11_USAGE_STAGING;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	textureDesc.BindFlags = 0;
	textureDesc.MiscFlags = 0;
	hr = pDevice->CreateTexture2D(&textureDesc, NULL, &pStaging);
	if (FAILED(hr))
	{
		return hr;
	}

	// 스테이징 텍스쳐 생성
	//hr = CreateStagingTexture2D(map, pContext);
	//if (FAILED(hr))
	//{
	//	return false;
	//}

	//hr = g_pd3dDevice->CreateRenderTargetView(pTexture, NULL, &m_pRTV);
	//if (FAILED(hr))
	//{
	//	return false;
	//}

	hr = g_pd3dDevice->CreateShaderResourceView(pTexture, NULL, &m_pSRV);
	if (FAILED(hr))
	{
		return false;
	}

	pContext->CopyResource(pTexture, pStaging);

	//if (pTexture)
	//{
	//	pTexture->Release();
	//}

	return hr;
}

bool HTextureMap::Frame(HMap* map, ID3D11DeviceContext* pContext)
{

	WriteTextureData(map, pContext);

	// D3D11_USAGE_STAGING를 사용하면 세이더 리소스뷰를 만들 수 없기 때문에
	// 새로운 텍스처에 복사한 후 리소스뷰를 생성
	pContext->CopyResource(pTexture, pStaging);

	return true;
}

void HTextureMap::WriteTextureData(HMap* map, ID3D11DeviceContext* pContext)
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC texDesc;
	pStaging->GetDesc(&texDesc);

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	hr = pContext->Map((ID3D11Resource*)pStaging, 0, D3D11_MAP_READ_WRITE, 0, &mappedResource);
	if (SUCCEEDED(hr))
	{
		BYTE* pDestBytes = (BYTE*)mappedResource.pData;

		float fWidthRatio = map->m_iNumCellRows / (float)texDesc.Width;
		float fHeightRatio = map->m_iNumCellCols / (float)texDesc.Height;

		for (UINT y = 0; y < texDesc.Height; y++)
		{

			BYTE* pDest = pDestBytes;

			for (UINT x = 0; x < texDesc.Width; x++)
			{
					int iRow = x * fWidthRatio;
					int iCol = y * fHeightRatio;

					float h = map->GetHeightMap(iCol, iRow);


					*pDest++ = h;
					*pDest++ = h;
					*pDest++ = h;
					*pDest++ = 255;

			}
			pDestBytes += mappedResource.RowPitch;
		}
		pContext->Unmap(pStaging, 0);
	}
}

void HTextureMap::WriteTextureDataAlphaZero(HMap* map, ID3D11DeviceContext* pContext)
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC texDesc;
	pStaging->GetDesc(&texDesc);

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	hr = pContext->Map((ID3D11Resource*)pStaging, 0, D3D11_MAP_READ_WRITE, 0, &mappedResource);
	if (SUCCEEDED(hr))
	{
		BYTE* pDestBytes = (BYTE*)mappedResource.pData;

		for (UINT y = 0; y < texDesc.Height; y++)
		{

			for (UINT x = 0; x < texDesc.Width; x++)
			{

				*pDestBytes++ = 0;
				*pDestBytes++ = 0;
				*pDestBytes++ = 0;
				*pDestBytes++ = 0;

			}
			pDestBytes += mappedResource.RowPitch;
		}
		pContext->Unmap(pStaging, 0);
	}
}

void HTextureMap::SetRadius(float radius)
{
	m_fRadius = radius;
}

void HTextureMap::PickRenderTextureData(HMap* map, ID3D11Texture2D* Texture2D, ID3D11DeviceContext* pContext, Vector3 pick)
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC texDesc;
	Texture2D->GetDesc(&texDesc);
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;

	if (pick.x == 0 && pick.z == 0)
	{
		return;
	}
	if (SUCCEEDED(pContext->Map((ID3D11Resource*)Texture2D,
		0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
		float fMapWidth = map->m_iNumCellCols * map->m_fCellDistance;
		float fWidthRatio = (float)texDesc.Width / fMapWidth;
		Vector2 vCenter = Vector2((pick.x + (fMapWidth / 2.0f)) * fWidthRatio,
			(-(pick.z - (fMapWidth / 2.0f))) * fWidthRatio);
		float fRadius = m_fRadius * fWidthRatio;
		
		if (beforePos != vCenter)
		{
			for (UINT y = 0; y < texDesc.Height; y++)
			{
				for (UINT x = 0; x < texDesc.Width; x++)
				{
					Vector2 p = Vector2(x, y);
					float fDist = (p - vCenter).Length();

					if (fDist < fRadius)
					{
						float iRatio = 0.0f;
						int iTemp = 0;

						if (fDist < fRadius / 2.0f)
						{
							iRatio = 255;
						}
						else
						{
							iRatio = ((1.0f - fDist / fRadius) * 2.0f) * 255;
						}

						iTemp = *pDestBytes + (int)iRatio;
						if (iTemp > 255) iTemp = 255;
						*pDestBytes = iTemp;
						pDestBytes += 4;

						continue;
					}
					pDestBytes += 4;
				}
			}
		}

		beforePos = vCenter;
		pContext->Unmap(Texture2D, 0);
	}

	// D3D11_TEXTURE2D_DESC desc;
	// pTexDest->GetDesc(&desc);
	// 
	// D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	// if (SUCCEEDED(pImmediateContext->Map((ID3D11Resource*)pTexDest,
	// 	0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	// {
	// 	BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
	// 	float fMapWidth = m_pMap->m_iNumCellCols * m_pMap->m_fCellDistance;
	// 	float fWidthRatio = (float)desc.Width / fMapWidth;
	// 	Vector2 vCenter = Vector2((vPick.x + (fMapWidth / 2.0f)) * fWidthRatio,
	// 		(-(vPick.z - (fMapWidth / 2.0f))) * fWidthRatio);
	// 	float fRadius = m_fOutRad * fWidthRatio;
	// 
	// 	for (UINT y = 0; y < desc.Height; y++)
	// 	{
	// 		for (UINT x = 0; x < desc.Width; x++)
	// 		{
	// 			Vector2 p = Vector2(x, y);
	// 			float fDist = (p - vCenter).Length();
	// 			if (fDist < fRadius)
	// 			{
	// 				float iRatio = 0.0f;
	// 				int iTemp = 0;
	// 				if (fDist < fRadius / 2.0f)
	// 				{
	// 					iRatio = 255;
	// 				}
	// 				else
	// 				{
	// 					iRatio = ((1.0f - fDist / fRadius) * 2.0f) * 255;
	// 				}
	// 				switch (m_eSplatType)
	// 				{
	// 				case SPLAT_TEX_01:
	// 				{
	// 					iTemp = *pDestBytes + (int)iRatio;
	// 					if (iTemp > 255) iTemp = 255;
	// 					*pDestBytes = iTemp;
	// 					pDestBytes += 4;
	// 				}
	// 				break;
	// 				case SPLAT_TEX_02:
	// 				{
	// 					pDestBytes++;
	// 					iTemp = *pDestBytes + (int)iRatio;
	// 					if (iTemp > 255) iTemp = 255;
	// 					*pDestBytes = iTemp;
	// 					pDestBytes += 3;
	// 				}
	// 				break;
	// 				case SPLAT_TEX_03:
	// 				{
	// 					pDestBytes += 2;
	// 					iTemp = *pDestBytes + (int)iRatio;
	// 					if (iTemp > 255) iTemp = 255;
	// 					*pDestBytes = iTemp;
	// 					pDestBytes += 2;
	// 				}
	// 				break;
	// 				case SPLAT_TEX_04:
	// 				{
	// 					pDestBytes += 3;
	// 					iTemp = *pDestBytes + (int)iRatio;
	// 					if (iTemp > 255) iTemp = 255;
	// 					*pDestBytes = iTemp;
	// 					pDestBytes++;
	// 				}
	// 				break;
	// 				default:
	// 					break;
	// 				}
	// 				continue;
	// 			}
	// 			pDestBytes += 4;
	// 		}
	// 	}
	// 	pImmediateContext->Unmap(pTexDest, 0);

}

HRESULT HTextureMap::SetDepthStencilView()
{
	// Depth Texture 생성
	ID3D11Texture2D* pTexture = nullptr;
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	textureDesc.Width = 512;
	textureDesc.Height = 512;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	HRESULT hr = g_pd3dDevice->CreateTexture2D(&textureDesc, NULL, &pTexture);
	if (FAILED(hr))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(pTexture, &dsvDesc, &m_pDSV);
	if (FAILED(hr))
	{
		return false;
	}

	if (pTexture)
	{
		pTexture->Release();
	}

	return true;
}

bool HTextureMap::SetViewport()
{
	m_RightViewPort.TopLeftX = 0;
	m_RightViewPort.TopLeftY = 0;
	m_RightViewPort.Width = 512;
	m_RightViewPort.Height = 512;
	m_RightViewPort.MinDepth = 0.0f;
	m_RightViewPort.MaxDepth = 1.0f;
	return true;
}

bool HTextureMap::CreateVertexData()
{
	m_VertexList.resize(4);
	m_VertexList =
	{
	   { PNCT_VERTEX(
		 Vector3(-1.0f, 1.0f, 0.0f),
		 Vector3(0.0f,0.0f,-1.0f),
		 Vector4(1.0f,1.0f,1.0f,1.0f),
		 Vector2(0.0f,0.0f))
	   },

	   { PNCT_VERTEX(
		 Vector3(-0.5f, 1.0f, 0.0f),
		 Vector3(0.0f,0.0f,-1.0f),
		 Vector4(1.0f,1.0f,1.0f,1.0f),
		 Vector2(1.0f,0.0f)) },

	   { PNCT_VERTEX(
		 Vector3(-0.5f, 0.5f, 0.0f),
		 Vector3(0.0f,0.0f,-1.0f),
		 Vector4(1.0f,1.0f,1.0f,1.0f),
		 Vector2(1.0f, 1.0f)) },

	   { PNCT_VERTEX(
		 Vector3(-1.0f, 0.5f, 0.0f),
		 Vector3(0.0f,0.0f,-1.0f),
		 Vector4(1.0f,1.0f,1.0f,1.0f),
		 Vector2(0.0f,1.0f)) },
	};

	return true;
}

bool HTextureMap::CreateIndexData()
{
	HShapePlane::CreateIndexData();

	return true;
}

bool HTextureMap::Create(HMap* map, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext ,T_STR szVS, T_STR szPS, T_STR filename)
{

	m_HDxRT.Set(pd3dDevice);
	g_pd3dDevice = pd3dDevice;
	SetRenderTargetView(pd3dDevice, pContext);
	SetDepthStencilView();
	SetViewport();
	HDxObject::Create(pd3dDevice, szVS, szPS, filename);

	return true;
}

void HTextureMap::ClearShaderResources(ID3D11DeviceContext * pd3dContext)
{
	ID3D11ShaderResourceView* pNullViews[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	pd3dContext->VSSetShaderResources(0, 8, pNullViews);
	pd3dContext->GSSetShaderResources(0, 8, pNullViews);
	pd3dContext->PSSetShaderResources(0, 8, pNullViews);
	pd3dContext->PSSetShaderResources(1, 8, pNullViews);
	pd3dContext->PSSetShaderResources(2, 8, pNullViews);
	pd3dContext->PSSetShaderResources(3, 8, pNullViews);
}

bool HTextureMap::Begin(ID3D11DeviceContext * pd3dContext)
{
	UINT iNumView = 1;

	pd3dContext->RSGetViewports(&iNumView, &m_SaveVIewPort);
	pd3dContext->OMGetRenderTargets(1, &m_pSaveRTV, &m_pSaveDSV);
	/*float clearColor[] = { cosf(g_fGameTimer)*0.5f + 0.5f,
							-cosf(g_fGameTimer)*0.5f + 0.5f,
							sinf(g_fGameTimer)*0.5f + 0.5f,1 };*/

							// 클리어
	ID3D11RenderTargetView* pNull = NULL;
	pd3dContext->OMSetRenderTargets(1, &pNull, NULL);
	ClearShaderResources(pd3dContext);

	// 클리어 후 새로 set
	pd3dContext->RSSetViewports(1, &m_RightViewPort);
	pd3dContext->OMSetRenderTargets(1, &m_pRTV, m_pDSV);
	float clearColor[] = { 0,0,0,1 };
	pd3dContext->ClearRenderTargetView(m_pRTV, clearColor);
	pd3dContext->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);


	return true;
}

bool HTextureMap::End(ID3D11DeviceContext * pd3dContext)
{
	UINT iNumView = 1;
	pd3dContext->OMSetRenderTargets(1, &m_pSaveRTV, m_pSaveDSV);
	pd3dContext->RSSetViewports(iNumView, &m_SaveVIewPort);

	m_pSaveRTV->Release();
	m_pSaveDSV->Release();

	return true;
}

bool HTextureMap::Render(ID3D11DeviceContext * pd3dContext)
{
	//pd3dContext->PSSetShaderResources(0, 1, &m_pSRV);
	//HShapePlane::Render(pd3dContext);

	HDxObject::Update(pd3dContext);
	HDxObject::PreRender(pd3dContext);

	UINT iStride = sizeof(PNCT_VERTEX);
	UINT iOffset = 0;
	pd3dContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &iStride, &iOffset);
	pd3dContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pd3dContext->IASetInputLayout(m_pInputLayout);

	pd3dContext->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	pd3dContext->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);

	pd3dContext->VSSetShader(m_pVertexShader, NULL, 0);
	pd3dContext->PSSetShader(m_pPixelShader, NULL, 0);
	pd3dContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iTopology);
	//pd3dContext->PSSetShaderResources(0, 1, &m_pSRV);

	PostRender(pd3dContext);
	//if (m_pTexture != nullptr)
	//{
	//	pd3dContext->PSSetShaderResources(0, 1, &m_pSRV);
	//}
	//if (m_pIndexBuffer == nullptr)
	//{
	//	pd3dContext->Draw(m_VertexList.size(), 0);
	//}
	//else
	//{
	//	//PostRender(pd3dContext);
	//}

	return true;
}

bool HTextureMap::PostRender(ID3D11DeviceContext * pd3dContext)
{
	pd3dContext->PSSetShaderResources(0, 1, &m_pSRV);
	HObject::PostRender(pd3dContext);
	//ClearShaderResources(pd3dContext);
	return true;
}

bool HTextureMap::Release()
{
	if (m_pRTV)
	{
		m_pRTV->Release();
	}

	if (m_pSRV)
	{
		m_pSRV->Release();
	}

	if (m_pDSV)
	{
		m_pDSV->Release();
	}

	if (pTexture)
	{
		pTexture->Release();
	}

	if (pStaging)
	{
		pStaging->Release();
	}

	return true;
}

HTextureMap::HTextureMap()
{
}

HTextureMap::~HTextureMap()
{
}
