#pragma once
#include "ModelObj.h"

template<class H> 
class SubMesh
{
public:
	int m_iNumFace;
	vector<H> m_VertexArray;
};

class Mesh : public ModelObj
{
public:
	Matrix m_matXFormToWorld;
	Mesh* m_pParent;
	int m_iNumFace;


	int m_iMtrlRef;


};

