#pragma once
#include "HObject.h"

struct HMapDesc
{
	int iNumCols;
	int iNumRows;
	float fCellDistance;
	T_STR szTextFile;
	T_STR szVS;
	T_STR szPS;
};

class HMap : public HObject
{
public:
	HMapDesc m_MapDesc;
	UINT m_iNumRows;
	UINT m_iNumCols;
	UINT m_iNumCellCols;
	UINT m_iNumCellRows;
	UINT m_iNumVertices;
	UINT m_iNumFaces;
	float m_fCellDistance;

public:
	bool CreateMap(ID3D11Device* pDevice, HMapDesc desc);
	virtual bool CreateVertexData();
	virtual bool CreateIndexData();
	virtual bool Frame();
	virtual bool PostRender(ID3D11DeviceContext* pd3dContext);

public:
	HMap();
	virtual ~HMap();
};

