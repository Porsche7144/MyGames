#pragma once
#include "HProjectTile.h"

class HGameUser : public HObject
{
public:
	HEffect*  m_pProjectTile;
	bool  Reset();
	vector<HProjectTileInfo> m_pProjectTileList;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;

	HGameUser()
	{
		m_iObjectType = 100;
	}
};

