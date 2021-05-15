//#pragma once
//#include <fbxsdk.h>
//#include "HObject.h"
//#include <unordered_map>
//
//struct IsTriangleSame
//{
//	HTriangle			m_Tri;
//	IsTriangleSame(HTriangle data) : m_Tri(data) {}
//	bool operator()(HTriangle& value)
//	{
//		return value.iSubIndex == m_Tri.iSubIndex;
//	}
//};
//
//struct HSubMesh
//{
//	std::vector<HTriangle>		m_TriangleList;
//	std::vector<PNCT_VERTEX>	m_VertexList;
//	std::vector<IW_VERTEX>		m_IWVertexList;
//	ComPtr<ID3D11Buffer>		m_pVertexBuffer;
//	ComPtr<ID3D11Buffer>        m_pIWVertexBuffer;
//	ComPtr<ID3D11Buffer>		m_pIndexBuffer;
//	HTexture*					m_pTexture;
//
//	std::vector<DWORD> m_IndexArray;
//	int m_iNumFace;
//	void SetUniqueBuffer(HTriangle& tri);
//
//	~HSubMesh()
//	{
//		m_iNumFace = 0;
//		m_pTexture = nullptr;
//	}
//
//};
//
//struct HWeight
//{
//	std::vector<int>   m_Index;
//	std::vector<float> m_Weight;
//	void InsertWeight(int iBoneIndex, float fBoneWeight)
//	{
//		for (DWORD i = 0; i < m_Index.size(); ++i)
//		{
//			if (fBoneWeight > m_Weight[i])
//			{
//				for (DWORD j = (m_Index.size() - 1); j > i; --j)
//				{
//					m_Index[j] = m_Index[j - 1];
//					m_Weight[j] = m_Weight[j - 1];
//				}
//				m_Index[i] = iBoneIndex;
//				m_Weight[i] = fBoneWeight;
//				break;
//			}
//		}
//	}
//	HWeight()
//	{
//		m_Index.resize(8);
//		m_Weight.resize(8);
//	}
//};
//
//struct HAnimTrack
//{
//	int	   iTick;
//	Vector3 p;
//	Vector3 s;
//	Quaternion q;
//	Matrix mat;
//};
//
//struct HScene
//{
//	int iStartFrame;
//	int iEndFrame;
//	int iFrameSpeed; // 30
//	int iTickPerFrame;// 160
//	int iNumMesh;
//	int iDeltaTick; // 1frame
//	float fDeltaTime;
//	float fStartTime;
//	float fEndTime;
//};
//
//class HModelObj : public HObject
//{
//public:
//	HModelObj* m_pParentObj = nullptr;
//	Matrix m_matAnim;
//
//	std::unordered_map<std::string, Matrix> m_dxMatrixBindPoseMap;
//	std::vector<std::wstring>	FbxMaterialList;
//	std::vector<HSubMesh>		m_SubMesh;
//	std::vector<HAnimTrack>		m_AnimList;
//	std::vector<HWeight>		m_WeightList;
//	bool						m_bSkinnedMesh = false;
//
//public:
//	bool CreateInputLayOut() override;
//
//public:
//	HModelObj() {}
//	virtual ~HModelObj() {}
//};
//
