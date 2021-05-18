#include "HheightMap.h"

float HheightMap::GetHeight(UINT index)
{
	// 원본의 높이값이 너무 높으므로 여기서 조절.
	return m_fHeightList[index] / m_MapDesc.fScaleHeight;
}

bool HheightMap::CreateHeightMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const TCHAR* pFilename)
{
	HRESULT hr;
	ID3D11Resource* pTexture;
	size_t maxsize = 0;

	// 불러온 Texture를 CPU가 읽어들일 수 있도록 D3D11_USAGE_STAGING,
	// D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ 로 할당.
	if (FAILED(hr = CreateWICTextureFromFileEx(pDevice,
		pFilename,
		maxsize,
		D3D11_USAGE_STAGING,
		0,
		D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,
		0,
		WIC_LOADER_DEFAULT,
		&pTexture, nullptr)))
	{

		if (FAILED(hr = CreateDDSTextureFromFileEx(pDevice,
			pFilename,
			maxsize,
			D3D11_USAGE_STAGING,
			0,
			D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,
			0,
			WIC_LOADER_DEFAULT,
			&pTexture, nullptr)))
			{
				return false;
			}		

	}

	ID3D11Texture2D* pTexture2D = NULL;
	// QueryInterface
	// 개체가 특정의 구성 요소 개체 모델 (COM) 인터페이스를 지원 하고 있을지 어떨지를 판별한다. 
	// 인터페이스가 지원 되고 있는 경우, 시스템은 개체의 참조 카운트를 늘려, 
	// 애플리케이션은, 그 인터페이스를 곧바로 사용할 수 있다.
	if (FAILED(pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D)))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC desc;
	// GetDesc
	// 버퍼에 대한 정보를 얻어온다.
	pTexture2D->GetDesc(&desc);

	m_fHeightList.resize((desc.Height) * (desc.Width));


	// 이미지의 RGBA에 따른 높이값을 계산한다.
	if (pTexture2D)
	{
		D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
		if (SUCCEEDED(pContext->Map((ID3D11Resource*)pTexture2D, 
			D3D11CalcSubresource(0, 0, 1), 
			D3D11_MAP_READ, 
			0, &MappedFaceDest)))
		{
			UCHAR* pTexels = (UCHAR*)MappedFaceDest.pData;
			PNCT_VERTEX v;
			for (UINT row = 0; row < desc.Height; row++)
			{
				UINT rowStart = row * MappedFaceDest.RowPitch;

				for (UINT col = 0; col < desc.Width; col++)
				{
					// * 바이트 수
					UINT colStart = col * 4;
					UINT uRed = pTexels[rowStart + colStart + 0];
					m_fHeightList[row * desc.Width + col] = uRed; // DWORD이므로 Pitch값 / 4

				}
			}
			pContext->Unmap(pTexture2D, D3D11CalcSubresource(0, 0, 1));
		}
	}

	m_iNumRows = desc.Height;
	m_iNumCols = desc.Width;
	pTexture2D->Release();
	pTexture->Release();

	return true;
}