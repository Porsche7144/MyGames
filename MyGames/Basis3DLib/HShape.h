#pragma once
#include "HObject.h"
#include "HDxObject.h"

class HShape : public HObject
{

public:
	HShape();
	virtual ~HShape();
};

class HShapeSphere : public HShape
{
public:
	HDxObject m_HDxObj;
public:
	bool CreateVertexData() override;
	bool CreateIndexData() override;
	void CreateSphere(UINT Slices, float fRadius);
	void MakeSpherePoint(std::vector<Vector3> &spherePoints, UINT Slices, double fRadius, Vector3 center);
	bool Draw(ID3D11DeviceContext * pContext);
public:
	HShapeSphere();
	virtual ~HShapeSphere();
};

class HShapeBox : public HShape
{
public:
	bool CreateVertexData() override;
	bool CreateIndexData() override;
public:
	HShapeBox();
	virtual ~HShapeBox();
};

class HShapePlane : public HShape
{
public:
	bool CreateVertexData() override;
	bool CreateIndexData() override;
public:
	HShapePlane();
	virtual ~HShapePlane();
};

class HShapeLine : public HShape
{
public:
	bool Draw(ID3D11DeviceContext* pContext, Vector3 p, Vector3 e, Vector4 c = Vector4(1, 0, 0, 1));

public:
	bool CreateVertexData() override;
	bool CreateIndexData() override;
public:
	HShapeLine();
	virtual ~HShapeLine();
};

