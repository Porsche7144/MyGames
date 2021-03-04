#pragma once
#include "HCamera.h"
#include "HMap.h"
#include <queue>

struct HNode
{
	DWORD   m_dwDepth;
	BOOL	m_isLeaf;
	UINT    m_CornerIndex[4];
	HNode*  m_Child[4];
	std::vector<DWORD>      m_IndexList;
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;
};

struct LTRB_POSITION
{
	float TopLeft;
	float TopRight;
	float BottomLeft;
	float BottomeRight;
};

class HQuadTree
{
	enum QuadTreeCorner
	{
		TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT,
	};

public:
	HMap* m_pMap;
	ID3D11Device* m_pd3dDevice;
	HNode* m_pParentNode;
	LTRB_POSITION m_ltPos[4];

	float m_DivideSize;

public:
	bool Build(HMap* map);
	HNode* CreateTreeNode(float topLeft, float topRight, float bottomLeft, float bottomRight);
	bool NodeDivide(HNode* pNode);
	bool SetChildTree(HNode* pNode);
	void DrawNode(ID3D11DeviceContext * pContext, HNode* pNode);
	void CreateIndexNode(HNode* pNode);
	bool Render(ID3D11DeviceContext* pContext);

public:
	HQuadTree(void);
	virtual ~HQuadTree(void);
};

