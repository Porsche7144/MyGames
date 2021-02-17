#include "HTexture.h"

bool  HTexture::Init()
{	
	return true;
}

bool  HTexture::Load(ID3D11Device* pd3dDevice, T_STR filename)
{
	ID3D11Resource* texture;
	HRESULT hr = DirectX::CreateWICTextureFromFile(pd3dDevice, filename.c_str(), NULL, &m_pTextureSRV);

	if (FAILED(hr)) return false;

	return true;
}

bool  HTexture::Frame()
{
	return true;
}

bool  HTexture::Render()
{
	return true;
}

bool  HTexture::Release()
{
	if (m_pTextureSRV)
	{
		m_pTextureSRV->Release();
		m_pTextureSRV = nullptr;
	}

	return true;
}

HTexture::HTexture()
{
	m_pTextureSRV = nullptr;
}

HTexture::~HTexture()
{
}