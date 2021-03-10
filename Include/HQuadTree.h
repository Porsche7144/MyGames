#pragma once
#include "HCamera.h"
#include "HMap.h"
#include <queue>

struct HNode
{
	DWORD   m_dwDepth;
	BOOL	m_isLeaf;
	UINT    m_CornerIndex[4];
	H_BOX m_hBox;
	H_SPHERE m_Sphere;
	HNode*  m_Child[4];
	std::vector<DWORD>      m_IndexList;
	std::vector<PNCT_VERTEX> m_VertexList;
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;
	ComPtr<ID3D11Buffer>	m_pVertexBuffer;
	HNode()
	{
		m_Child[0] = nullptr;
		m_Child[1] = nullptr;
		m_Child[2] = nullptr;
		m_Child[3] = nullptr;
	}
	~HNode()
	{
		if (m_Child[0] != nullptr)
		{
			for (int i = 0; i < 4; i++)
			{
				delete m_Child[i];
			}
		}
	}
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
	HDxObject m_DXObj;
	HMap* m_pMap;
	ID3D11Device* m_pd3dDevice;
	HNode* m_pParentNode;
	LTRB_POSITION m_ltPos[4];
	std::vector<HNode*> m_leafList;

	float m_DivideSize;

public:
	bool Build(HMap* map);
	HNode* CreateTreeNode(float topLeft, float topRight, float bottomLeft, float bottomRight);
	bool NodeDivide(HNode* pNode);
	bool SetChildTree(HNode* pNode);
	void DrawNode(ID3D11DeviceContext * pContext, HNode* pNode);
	void Draw(ID3D11DeviceContext * pContext, HNode* pNode);
	void CreateIndexNode(HNode* pNode);
	void CreateVertexNode(HNode* pNode);
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();

public:
	HQuadTree(void);
	virtual ~HQuadTree(void);
};

