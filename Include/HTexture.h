#pragma once
#include "HStd.h"

class HTexture
{
public:
	ID3D11ShaderResourceView* m_pTextureSRV;

public:
	bool  Init();
	bool  Frame();
	bool  Render();
	bool  Release();
public:
	bool  Load(ID3D11Device* pd3dDevice, T_STR filename);
public:
	HTexture();
	virtual ~HTexture();
};

