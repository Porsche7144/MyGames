#pragma once
#include "HScene.h"
class HSceneInGame : public HScene
{
public:
	bool m_bGameOver;

public:
	bool Frame();

public:
	HSceneInGame();
};

