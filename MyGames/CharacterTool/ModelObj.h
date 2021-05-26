#pragma once
#include <fbxsdk.h>
#include <unordered_map>
#include "HObject.h"
class Mesh;
class ModelObj;

class ModelObj : public HObject
{
public:
	vector<shared_ptr<Mesh>> m_pMesh;

public:
	T_STR m_szName;
};

