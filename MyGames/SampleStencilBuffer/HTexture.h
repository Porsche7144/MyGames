#pragma once
#include "HStd.h"

class HTexture
{
public:

public:
	bool  Init();
	bool  Frame();
	bool  Render();
	bool  Release();
public:
	bool  Load(wstring filename);
public:
	HTexture();
	virtual ~HTexture();
};

