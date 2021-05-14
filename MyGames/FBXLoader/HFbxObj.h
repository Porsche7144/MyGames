#pragma once
#include <fbxsdk.h>
#include "HObject.h"
#include <unordered_map>

using namespace DirectX::SimpleMath;

struct HSubMesh
{
	std::vector<HTriangle>		m_TriangleList;
	std::vector<PNCT_VERTEX>	m_VertexList;
	ComPtr<ID3D11Buffer>		m_pVertexBuffer;
	HTexture*					m_pTexture;
	int iCount;
	~HSubMesh()
	{
		// m_pTexture = nullptr;
	}

};

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

struct HWeight
{
	std::vector<int>   m_Index;
	std::vector<float> m_Weight;
	void InsertWeight(int iBoneIndex, float fBoneWeight)
	{
		for (DWORD i = 0; i < m_Index.size(); ++i)
		{
			if (fBoneWeight > m_Weight[i])
			{
				for (DWORD j = (m_Index.size() - 1); j > i; --j)
				{
					m_Index[j] = m_Index[j - 1];
					m_Weight[j] = m_Weight[j - 1];
				}
				m_Index[i] = iBoneIndex;
				m_Weight[i] = fBoneWeight;
				break;
			}
		}
	}
	HWeight()
	{
		m_Index.resize(8);
		m_Weight.resize(8);
	}
};

struct HAnimTrack
{
	int	   iTick;
	Matrix mat;
};

struct HScene
{
	int iStartFrame;
	int iEndFrame;
	int iFrameSpeed; // 30
	int iTickPerFrame;// 160
	int iNumMesh;
	int iDeltaTick; // 1frame
	float fDeltaTime;
	float fStartTime;
	float fEndTime;
};

class SkinData
{
public:
	static int				iNumMaxWeight;
	std::vector<FbxNode*>	InfluenceNodes;
	std::unordered_map<T_STR, Matrix> m_matBindPoseMap;

	size_t					dwVertexCount;
	DWORD					dwVertexStride;
	std::unique_ptr<int[]>	pBoneIndices;
	std::unique_ptr<float[]> pBoneWeights;
public:
	void	Alloc(size_t dwCount, DWORD dwStride);
	int*	GetIndices(size_t dwIndex);
	float*	GetWeights(size_t dwIndex);
	DWORD	GetBoneCount() const;
	void	InsertWeight(size_t dwIndex, DWORD dwBoneIndex, float fBoneWeight);
public:
	SkinData() : dwVertexCount(0), dwVertexStride(0) {}
	~SkinData() {}
};

class HModelObject : public HObject
{
public:
	std::vector<std::wstring>	FbxMaterialList;
	std::vector<HSubMesh>		m_SubMesh;
	std::vector<HAnimTrack>		m_AnimList;
	std::vector<HWeight>		m_WeightList;
	bool						m_bSkinnedMesh = false;

	virtual ~HModelObject()
	{
		// m_SubMesh.clear();
	}
};

typedef std::unordered_map<FbxNode*, HModelObject*> hMeshMap;
typedef std::vector<HModelObject*>	hMeshList;

class HFbxObj
{
public:
	HScene    m_AnimScene;
	float	  m_fTick = 0.0f;
	static FbxManager*	g_pSDKManager;
	FbxImporter*		m_pFbxImporter;
	FbxScene*			m_pFbxScene;
	FbxNode*			m_pFbxRootNode;
	std::unordered_map<std::string, Matrix> m_dxMatrixMap;
	std::unordered_map<FbxNode*, int> m_pNodeMap;
	std::vector<Matrix> m_MatrixList;
	hMeshMap m_hMeshMap;
	hMeshList m_hMeshList;

public:
	bool Load(std::string FileName);
	bool LoadFBX(std::string FileName);
	bool Initialize(std::string FileName);
	void PreProcess(FbxNode* pNode);
	void ParseNode(FbxNode* pNode, Matrix matParent);
	void ParseAnimation(FbxScene * pScene);
	void ParseMesh(FbxNode* pNode, FbxMesh* pMesh, HModelObject* pObj);
	Matrix ParseTransform(FbxNode* pNode, Matrix& matParentWorld);
	void ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet,	int vertexIndex, int uvIndex, FbxVector2& uv);
	FbxColor ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* pVertexColorSet,
		DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector4 ReadNormal(const FbxMesh* mesh, int controlPointIndex, int VertexCounter);
	std::string ParseMaterial(FbxSurfaceMaterial* pMtrl);
	bool ParseMeshSkinning(const FbxMesh * pMesh, SkinData * skindata);
	bool ParseMeshSkinningMap(const FbxMesh * pMesh, std::vector<HWeight>& skindata);
	void ParseNodeAnimation(FbxNode* pNode);
	void ParseAnimStack(FbxScene* pScene, FbxString* pStrAnimStackName);

public:
	HFbxObj();
	virtual ~HFbxObj();
};

