#include "HShape.h"

HShape::HShape()
{
}

HShape::~HShape()
{
}


bool HShapeBox::CreateVertexData()
{
	m_VertexList.resize(24);

	m_VertexList =
	{
		// 앞면
	   { PNCT_VERTEX(Vector3(-1.0f, 1.0f, -1.0f),
		 Vector3(0,0,0),
		 Vector4(1,0,0,1),
		 Vector2(0,0))
	   },

	   { PNCT_VERTEX(Vector3(1.0f, 1.0f, -1.0f),
		 Vector3(0,0,0),
		 Vector4(0,1,0,1),
		 Vector2(1,0)) },

	   { PNCT_VERTEX(Vector3(1.0f, -1.0f, -1.0f),
		 Vector3(0,0,0),
		 Vector4(0,0,1,1),
		 Vector2(0,1)) },

	   { PNCT_VERTEX(Vector3(-1.0f, -1.0f, -1.0f),
		 Vector3(0,0,0),
		 Vector4(1,1,1,1),
		 Vector2(1,1)) },

		// 뒷면
	   { PNCT_VERTEX(Vector3(1.0f, 1.0f, 1.0f),
		 Vector3(0,0,0),
		 Vector4(1,0,0,1),
		 Vector2(0,0))
	   },

	   { PNCT_VERTEX(Vector3(-1.0f, 1.0f, 1.0f),
		 Vector3(0,0,0),
		 Vector4(0,1,0,1),
		 Vector2(1,0)) },

	   { PNCT_VERTEX(Vector3(-1.0f, -1.0f, 1.0f),
		 Vector3(0,0,0),
		 Vector4(0,0,1,1),
		 Vector2(0,1)) },

	   { PNCT_VERTEX(Vector3(1.0f, -1.0f, 1.0f),
		 Vector3(0,0,0),
		 Vector4(1,1,1,1),
		 Vector2(1,1)) },

		// 위
		{ PNCT_VERTEX(Vector3(-1.0f, 1.0f, 1.0f),
		  Vector3(0,0,0),
		  Vector4(1,0,0,1),
		  Vector2(0,0))
		},

		{ PNCT_VERTEX(Vector3(1.0f, 1.0f, 1.0f),
		  Vector3(0,0,0),
		  Vector4(0,1,0,1),
		  Vector2(1,0)) },

		{ PNCT_VERTEX(Vector3(1.0f, 1.0f, -1.0f),
		  Vector3(0,0,0),
		  Vector4(0,0,1,1),
		  Vector2(0,1)) },

		{ PNCT_VERTEX(Vector3(-1.0f, 1.0f, -1.0f),
		  Vector3(0,0,0),
		  Vector4(1,1,1,1),
		  Vector2(1,1)) },

		// 아래
		{ PNCT_VERTEX(Vector3(1.0f, -1.0f, 1.0f),
		  Vector3(0,0,0),
		  Vector4(1,0,0,1),
		  Vector2(0,0))
		},

		{ PNCT_VERTEX(Vector3(-1.0f, -1.0f, 1.0f),
		  Vector3(0,0,0),
		  Vector4(0,1,0,1),
		  Vector2(1,0)) },

		{ PNCT_VERTEX(Vector3(-1.0f, -1.0f, -1.0f),
		  Vector3(0,0,0),
		  Vector4(0,0,1,1),
		  Vector2(0,1)) },

		{ PNCT_VERTEX(Vector3(1.0f, -1.0f, -1.0f),
		  Vector3(0,0,0),
		  Vector4(1,1,1,1),
		  Vector2(1,1)) },

		// 왼쪽
		{ PNCT_VERTEX(Vector3(-1.0f, 1.0f, 1.0f),
		  Vector3(0,0,0),
		  Vector4(1,0,0,1),
		  Vector2(0,0))
		},

		{ PNCT_VERTEX(Vector3(-1.0f, 1.0f, -1.0f),
		  Vector3(0,0,0),
		  Vector4(0,1,0,1),
		  Vector2(1,0)) },

		{ PNCT_VERTEX(Vector3(-1.0f, -1.0f, -1.0f),
		  Vector3(0,0,0),
		  Vector4(0,0,1,1),
		  Vector2(0,1)) },

		{ PNCT_VERTEX(Vector3(-1.0f, -1.0f, 1.0f),
		  Vector3(0,0,0),
		  Vector4(1,1,1,1),
		  Vector2(1,1)) },

		// 오른쪽
		{ PNCT_VERTEX(Vector3(1.0f, 1.0f, -1.0f),
		  Vector3(0,0,0),
		  Vector4(1,0,0,1),
		  Vector2(0,0))
		},

		{ PNCT_VERTEX(Vector3(1.0f, 1.0f, 1.0f),
		  Vector3(0,0,0),
		  Vector4(0,1,0,1),
		  Vector2(1,0)) },

		{ PNCT_VERTEX(Vector3(1.0f, -1.0f, 1.0f),
		  Vector3(0,0,0),
		  Vector4(0,0,1,1),
		  Vector2(0,1)) },

		{ PNCT_VERTEX(Vector3(1.0f, -1.0f, -1.0f),
		  Vector3(0,0,0),
		  Vector4(1,1,1,1),
		  Vector2(1,1)) },

	};

	return true;
}

bool HShapeBox::CreateIndexData()
{
	m_IndexList.resize(36);

	m_IndexList =
	{
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7,
		8, 9, 10,
		8, 10, 11,
		12, 13, 14,
		12, 14, 15,
		16, 17, 18,
		16, 18, 19,
		20, 21, 22,
		20, 22, 23,
	};

	return true;
}

HShapeBox::HShapeBox()
{
}

HShapeBox::~HShapeBox()
{
}

bool HShapePlane::CreateVertexData()
{
	m_VertexList.resize(4);
	m_VertexList =
	{
	   { PNCT_VERTEX(Vector3(-1.0f, 1.0f, -1.0f),
		 Vector3(0.0f,0.0f,-1.0f),
		 Vector4(1.0f,1.0f,1.0f,1.0f),
		 Vector2(0.0f,0.0f))
	   },

	   { PNCT_VERTEX(Vector3(1.0f, 1.0f, -1.0f),
		 Vector3(0.0f,0.0f,-1.0f),
		 Vector4(1.0f,1.0f,1.0f,1.0f),
		 Vector2(1.0f,0.0f)) },

	   { PNCT_VERTEX(Vector3(1.0f, -1.0f, -1.0f),
		 Vector3(0.0f,0.0f,-1.0f),
		 Vector4(1.0f,1.0f,1.0f,1.0f),
		 Vector2(1.0f, 1.0f)) },

	   { PNCT_VERTEX(Vector3(-1.0f, -1.0f, -1.0f),
		 Vector3(0.0f,0.0f,-1.0f),
		 Vector4(1.0f,1.0f,1.0f,1.0f),
		 Vector2(0.0f,1.0f)) },
	};

	return true;
}

bool HShapePlane::CreateIndexData()
{
	m_IndexList.resize(6);
	m_IndexList =
	{
		0, 1, 2,
		0, 2, 3,
	};

	return true;
}

HShapePlane::HShapePlane()
{
}

HShapePlane::~HShapePlane()
{
}

bool HShapeLine::Draw(ID3D11DeviceContext * pContext, Vector3 p, Vector3 e, Vector4 c)
{
	m_VertexList = 
	{
	   { PNCT_VERTEX(p,
		 Vector3(0.0f,0.0f,-1.0f),
		 c,
		 Vector2(0,0))
	   },

	   { PNCT_VERTEX(e,
		 Vector3(0.0f,0.0f,-1.0f),
		 c,
		 Vector2(1,0)) },
	};
	pContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);

	return HShape::Render(pContext);
}

bool HShapeLine::CreateVertexData()
{
	m_VertexList.resize(2);
	m_VertexList =
	{
	   { PNCT_VERTEX(Vector3(0.0f, 0.0f, 0.0f),
		 Vector3(0.0f, 0.0f, -1.0f),
		 Vector4(1.0f ,0.0f ,0.0f ,1.0f),
		 Vector2(0,0))
	   },

	   { PNCT_VERTEX(Vector3(100.0f, 0.0f, 0.0f),
		 Vector3(0.0f, 0.0f, -1.0f),
		 Vector4(1.0f ,0.0f ,0.0f ,1.0f),
		 Vector2(1.0f, 0)) },
	};

	return true;
}

bool HShapeLine::CreateIndexData()
{
	m_IndexList.resize(2);
	m_IndexList =
	{
		0, 1
	};
	return true;
}

HShapeLine::HShapeLine()
{
	m_szPixelShader = "PSLine";
	m_iTopology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
}

HShapeLine::~HShapeLine()
{
}
