#pragma once
#include "HNode.h"
#include "HCamera.h"
#include "HMap.h"
#include <queue>

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
	ID3D11Device* m_pd3dDevice;
	HNode* m_pParentNode;
	LTRB_POSITION m_ltPos[4];

	float m_DivideSize;

public:
	bool Build(HMap& map);
	HNode* CreateTreeNode(float topLeft, float topRight, float bottomLeft, float bottomRight);
	bool NodeDivide(HNode* pNode);
	bool SetTree(HNode* pNode);
	Vector3 operator +(float fValue);
	bool Render(ID3D11DeviceContext* pContext);

public:
	HQuadTree(void);
	virtual ~HQuadTree(void);
};

