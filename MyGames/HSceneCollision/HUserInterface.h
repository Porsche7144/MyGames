#pragma once
#include "BitmapObject.h"
#include "HInput.h"

class HUserInterface : public BitmapObject
{
public:
	int m_iStateValue;

public:
	HUserInterface();
	virtual ~HUserInterface();
};

class HButton : public HUserInterface
{
	bool Frame();
	bool Render();
};
