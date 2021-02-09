#pragma once
#include "HStd.h"

class HDxState
{
public:
	static ID3D11RasterizerState*		m_pRSSSolidBack;
	static ID3D11RasterizerState*		m_pRSWireBack;
	static ID3D11RasterizerState*		m_pRS;
	static D3D11_FILL_MODE				m_FillMode;
	static D3D11_CULL_MODE				m_CullMode;
	static ID3D11SamplerState*			m_pWrapLinear;
	static ID3D11DepthStencilState*		m_pDSS;

public:
	static bool Set(ID3D11Device* pDevice);
	static bool SetRasterState(ID3D11Device* pDevice);
	static bool Release();
};

