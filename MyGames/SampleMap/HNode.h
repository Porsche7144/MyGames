#pragma once
#include "HFrustum.h"

class HBaseObj
{
public:
	H_BOX m_hBox;

public:
	HBaseObj() {};
	virtual ~HBaseObj() {};
};

class H_BoxObject : public HBaseObj
{
public:
	Matrix			m_matWorld;
	Vector3			m_vPosition;
	Vector4			m_vColor;
public:
	H_BoxObject();
};

class HNode
{
public:
	DWORD m_dwDepth;
	BOOL m_isLeaf;
	BOOL m_isTile;

	// �ٿ�� �ڽ�
	H_BOX m_hBox;
	vector<DWORD> m_CornerIndex;

	// ����� �ڳ� ���� �ε��� �� ��ġ
	vector<Vector3> m_CornerList;
	vector<HNode*> m_ChildList;
	vector<HNode* > m_NeighborList;
	vector<HBaseObj*> m_ObjectList;

	// �߰�
	DWORD m_dwPositionIndex[2]; // x, y

	// D3D11_BOX
	// 3D ���ڸ� ����
	D3D11_BOX m_BoxVB;
	D3D11_BOX m_BoxIB;

	UINT m_iBeginVB;
	UINT m_iBeginIB;
	vector<DWORD> m_IndexLIst;
	vector<PNCT_VERTEX> m_VertexList;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;

public:
	HNode();
	virtual ~HNode();


};

