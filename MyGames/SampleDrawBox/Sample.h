#pragma once
#include "HCore.h"
#include "HVector.h"
#include "WICTextureLoader.h"
#include "HMatrix.h"
#include "HShape.h"
#pragma comment(lib, "directxtk.lib")

class Sample : public HCore
{
public:
	HShapeBox m_Box;
	HShapePlane m_Plane;

public:
	HVector3					m_vCameraPos = { -5,-3,0 };
	HVector3					m_vCameraTarget = { 0,0,0 };

	HMatrix						m_matBoxWorld;
	HMatrix						m_matPlaneWorld;

	HMatrix						m_matView;
	HMatrix						m_matProject;

public:
	ID3D11RasterizerState*		m_pRSSSolidBack;
	ID3D11RasterizerState*		m_pRSWireBack;
	ID3D11RasterizerState*		m_pRS;
	D3D11_FILL_MODE				m_FillMode;
	D3D11_CULL_MODE				m_CullMode;

	ID3D11SamplerState*			m_pWrapLinear;

	ID3D11DepthStencilView*		m_pDSV;
	ID3D11DepthStencilState*	m_pDSS;

public:
	bool Init() override;
	bool Frame() override;
	bool PreRender() override;
	bool Render() override;
	bool Release() override;

public:
	void CompilerCheck(ID3DBlob* ErrorMsg);
	void SetRasterState();

};

HGAME_RUN;