#pragma once
#include "HNode.h"
#include "HCamera.h"
#include <queue>

class HQuadTree
{
public:
	ID3D11Device* m_pd3dDevice;
	enum QUADTREECORNER
	{
		TL = 0, TR = 1, BL = 2, BR = 3,
	};

	int m_iRenderDepth;
	HNode* m_pRootNode;
	queue<HNode*> m_pQuadTreeQueue;
	vector<HBaseObj*> m_DrawObjList;
	vector<HNode*> m_DrawNodeList;
	HCamera* m_pCamera;

	bool m_bUsedIndexList; // 정점 인덱스 사용해 공간분할
	int m_iMaxDepthLimit; // 노드 분할의 최대 한도
	int m_iMaxDepth;	  // 현 리프 노드의 레벨(깊이) 갯수
	float m_fWidth; 
	float m_fHeight;

	// 최소 분할 크기
	float m_fMinDivideSize;
	void SetMinDivideSize(int fMinDivideSize) { m_fMinDivideSize = (float)fMinDivideSize; };

public:
	void SetRenderDepth(DWORD dwDepth);
	void SetMaxDepthLimit(int iMaxDepth) { m_iMaxDepthLimit = iMaxDepth; };
	bool Build(float fWidth, float fHeight);
	HNode* CreateNode(HNode* pParentNode, float fTopLeft, float fTopRight, float fBottomLeft, float fBottomRight);
	bool BuildTree(HNode* pNode);
	bool SubDivide(HNode* pNode);
	int AddObject(HBaseObj* pObj);
	int CheckRect(HNode* pNode, HBaseObj* pObj);
	HNode* FindNode(HNode* pNode, HBaseObj* pObj);
	void DrawFindNode(HNode* pNode);
	void VisibleNode(HNode* pNode);
	void VisibleObject(HNode* pNode);
	void FindNeighborNode(HNode* pNode);
	void SetNeighborNode(HNode* pNode);
	DWORD CheckSize(DWORD dwSize);

public:
	bool PreFrame();
	bool Frame();
	bool Release();

public:
	HQuadTree(void);
	virtual ~HQuadTree(void);
};

