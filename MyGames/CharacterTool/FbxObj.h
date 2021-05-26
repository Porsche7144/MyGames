#pragma once
#include "ModelObj.h"
#include "Mesh.h"
#include "FbxMaterial.h"

static Matrix DxConvertMatrix(Matrix m)
{
	Matrix mat;

	mat._11 = m._11; mat._12 = m._13; mat._13 = m._12;
	mat._21 = m._31; mat._22 = m._33; mat._23 = m._32;
	mat._31 = m._21; mat._32 = m._23; mat._33 = m._22;
	mat._41 = m._41; mat._42 = m._43; mat._43 = m._42;

	mat._14 = mat._24 = mat._34 = 0.0f;
	mat._44 = 1.0f;

	return mat;
};

static Matrix ConvertMatrixA(const FbxMatrix& matrix)
{
	Matrix matResult;
	auto fData = reinterpret_cast<float*>(&matResult);
	auto pSrcData = reinterpret_cast<const DOUBLE*>(&matrix);
	for (DWORD i = 0; i < 16; ++i)
	{
		fData[i] = (float)pSrcData[i];
	}
	return matResult;
};

typedef std::unordered_map<T_STR, Matrix>		MatrixMap;
typedef SubMesh<PNCTIW_VERTEX>					FbxData;
typedef std::vector<shared_ptr<FbxData>>		FbxMeshData;
typedef std::unordered_map<FbxNode*, FbxMatrix> BindPoseMap;
typedef std::unordered_map<FbxNode*, Mesh*>		MeshMap;

class FbxObj : public ModelObj
{
public:
	static FbxManager*  g_pSDKManager;
	FbxImporter*		m_FbxImporter;
	FbxScene*			m_FbxScene;

public:
	MatrixMap		m_matFrameMatrixMap;
	BindPoseMap		m_BindPoseMap;
	FbxMeshData		m_pMeshData;
	MeshMap			m_MeshMap;

public:
	bool Load(ID3D11Device* pDevice, const char* fbxFileName);
	bool LoadFBX(ID3D11Device* pDevice, const char* fbxFileName);
	bool PreProcess(FbxNode* pNode);
	bool ParseNode(FbxNode* pNode, FbxScene* pScene, Mesh* pParent, const Matrix& matParentWorld);
	Matrix ParseTransform(FbxNode* pNode, const Matrix& matParentWorld);
	FbxAMatrix GetGeometryTransformation(FbxNode* pNode);
	void ParseMesh(FbxNode* pNode, FbxMesh* pFbxMesh, Mesh pMesh, FbxData pData);


public:
	FbxObj();
	~FbxObj();
};

