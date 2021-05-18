#include "HheightMap.h"

float HheightMap::GetHeight(UINT index)
{
	// ������ ���̰��� �ʹ� �����Ƿ� ���⼭ ����.
	return m_fHeightList[index] / m_MapDesc.fScaleHeight;
}

bool HheightMap::CreateHeightMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const TCHAR* pFilename)
{
	HRESULT hr;
	ID3D11Resource* pTexture;
	size_t maxsize = 0;

	// �ҷ��� Texture�� CPU�� �о���� �� �ֵ��� D3D11_USAGE_STAGING,
	// D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ �� �Ҵ�.
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
	// ��ü�� Ư���� ���� ��� ��ü �� (COM) �������̽��� ���� �ϰ� ������ ����� �Ǻ��Ѵ�. 
	// �������̽��� ���� �ǰ� �ִ� ���, �ý����� ��ü�� ���� ī��Ʈ�� �÷�, 
	// ���ø����̼���, �� �������̽��� ��ٷ� ����� �� �ִ�.
	if (FAILED(pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D)))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC desc;
	// GetDesc
	// ���ۿ� ���� ������ ���´�.
	pTexture2D->GetDesc(&desc);

	m_fHeightList.resize((desc.Height) * (desc.Width));


	// �̹����� RGBA�� ���� ���̰��� ����Ѵ�.
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
					// * ����Ʈ ��
					UINT colStart = col * 4;
					UINT uRed = pTexels[rowStart + colStart + 0];
					m_fHeightList[row * desc.Width + col] = uRed; // DWORD�̹Ƿ� Pitch�� / 4

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