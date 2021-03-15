#pragma once
#include "HCore.h"
#include "HFbxObj.h"

class Sample : public HCore
{
public:
	HFbxObj		m_FbxObj;

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;

};

HGAME_RUN;