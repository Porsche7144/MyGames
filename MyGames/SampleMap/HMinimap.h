#pragma once
#include "HShape.h"
#include "HDxRT.h"

class HMinimap : public HShapePlane
{
public:
	HDxRT m_HDxRT;

public:
	bool Create(ID3D11Device* pd3dDevice, T_STR szVS, T_STR szPS, T_STR filename) override;
	bool CreateVertexData();
	bool CreateIndexData();

public:
	virtual bool	Begin(ID3D11DeviceContext* pd3dContext);
	virtual bool	End(ID3D11DeviceContext* pd3dContext);
	virtual bool	Release() override;
	virtual bool	Render(ID3D11DeviceContext* pd3dContext) override;
	virtual bool	PostRender(ID3D11DeviceContext* pd3dContext) override;

public:
	HMinimap();
	virtual ~HMinimap();

};
