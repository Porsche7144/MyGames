#pragma once
#include "HMap.h"

class HheightMap :	public HMap
{
public:
	float GetHeight(UINT index) override;
	bool CreateHeightMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const TCHAR* pFilename);
};

