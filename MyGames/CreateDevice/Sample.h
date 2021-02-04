#pragma once
#include "HCore.h"

class Sample : public HCore
{
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	//virtual HRESULT DeleteDXResource();
	//virtual HRESULT CreateDXResource(UINT w, UINT h);

};
HGAME_RUN;

