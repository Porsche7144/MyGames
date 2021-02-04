#pragma once
#include "HScene.h"
class HSceneInGame : public HScene
{
public:
	bool m_bGameOver;

public:
	bool  CreateScene();
	bool  Reset();
	bool Frame();
	bool Render();

public:
	HSceneInGame();
};

