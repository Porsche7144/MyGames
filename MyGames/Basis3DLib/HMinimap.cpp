#include "HMinimap.h"

bool HMinimap::CreateVertexData()
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

bool HMinimap::CreateIndexData()
{
	HShapePlane::CreateIndexData();

	return true;
}

bool HMinimap::Create(ID3D11Device* pd3dDevice, T_STR szVS, T_STR szPS, T_STR filename)
{
	m_HDxRT.Set(pd3dDevice);
	g_pd3dDevice = pd3dDevice;
	m_HDxRT.SetRenderTargetView();
	m_HDxRT.SetDepthStencilView();
	m_HDxRT.SetViewport();
	HDxObject::Create(pd3dDevice, szVS, szPS, filename);

	return true;
}

bool HMinimap::Begin(ID3D11DeviceContext * pd3dContext)
{
	m_HDxRT.Begin(pd3dContext);
	return true;
}

bool HMinimap::End(ID3D11DeviceContext * pd3dContext)
{
	m_HDxRT.End(pd3dContext);
	return true;
}

bool HMinimap::Release()
{
	m_HDxRT.Release();
	return HObject::Release();
}

bool HMinimap::Render(ID3D11DeviceContext * pd3dContext)
{
	HShapePlane::Render(pd3dContext);
	return true;
}

bool HMinimap::PostRender(ID3D11DeviceContext * pd3dContext)
{
	pd3dContext->PSSetShaderResources(0, 1, &m_HDxRT.m_pSRV);
	HObject::PostRender(pd3dContext);
	m_HDxRT.ClearShaderResources(pd3dContext);
	return true;
}

HMinimap::HMinimap()
{
}

HMinimap::~HMinimap()
{
}
