#pragma once
#include "HShape.h"

#define MAX_SKY_TEXTURE 6

class HSky : public HShapeBox
{
public:
	ComPtr<ID3D11ShaderResourceView> m_pTexSRV[MAX_SKY_TEXTURE];
	ComPtr<ID3D11PixelShader> m_pSkyPS;
	bool m_bRenderType;

public:
	bool Render(ID3D11DeviceContext* pContext);
	bool PostRender(ID3D11DeviceContext* pContext);
	bool Create(ID3D11Device * pDevice, T_STR vs, T_STR ps, T_STR texture);
	bool LoadTexture(T_STR texture);

public:
	HSky()
	{
		m_bRenderType = false;
	}
	~HSky() {}

};

