#include "HNode.h"


HNode::HNode()
{
	m_dwDepth = 0;
	m_isLeaf = FALSE;
	m_hBox.vCenter = Vector3(0.0f, 0.0f, 0.0f);
	m_hBox.vMax = Vector3(1.0f, 1.0f, 1.0f);
	m_hBox.vMin = Vector3(-1.0f, -1.0f, -1.0f);
	m_hBox.vAxis[0] = Vector3(1.0f, 0.0f, 0.0f);
	m_hBox.vAxis[1] = Vector3(0.0f, 1.0f, 0.0f);
	m_hBox.vAxis[2] = Vector3(0.0f, 0.0f, 1.0f);
	m_hBox.fExtent[0] = 1;
	m_hBox.fExtent[1] = 1;
	m_hBox.fExtent[2] = 1;
}

HNode::~HNode()
{
	// 노드의 코너 정점 인덱스 및 위치
	if (!m_CornerList.empty())
	{
		m_CornerList.clear();
	}
	if (!m_CornerIndex.empty())
	{
		m_CornerIndex.clear();
	}

	for (int i = 0; i < m_ChildList.size(); i++)
	{
		SAFE_DEL(m_ChildList[i]);
	}
}
