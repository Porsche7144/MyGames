#pragma once
#include "HNode.h"
#include "HCamera.h"
#include "HMap.h"
#include <queue>

class HQuadTree
{
public:
	ID3D11Device* m_pd3dDevice;

public:
	bool Build(HMap& map);
	void CreateTreeNode(HNode* pNode, float topLeft, float topRight, float bottomLeft, float bottomRight);

public:
	HQuadTree(void);
	virtual ~HQuadTree(void);
};

