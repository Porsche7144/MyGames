#include "HQuadTree.h"


 // 0 31 63

bool HQuadTree::Build(HMap& map)
{
	float width = map.m_iNumCols;
	float Height = map.m_iNumRows;
	// Á¤Á¡ °¹¼ö
	int VertexCount = map.m_iNumVertices;
	// Á¤Á¡ÀÇ »ï°¢Çü °¹¼ö
	int TriangleCount = map.m_iNumVertices / 3;


	CreateTreeNode(NULL, -width / 2, width / 2, -Height / 2, Height / 2);

	return true;
}

void HQuadTree::CreateTreeNode(HNode* pNode, float topLeft, float topRight, float bottomLeft, float bottomRight)
{

}

HQuadTree::HQuadTree(void)
{
}

HQuadTree::~HQuadTree(void)
{
}
