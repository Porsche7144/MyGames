#pragma once
#include "HObject.h"
#include "HInput.h"

class HUserInterface : public HObject
{
public:
	int m_iStateValue;

public:

	HUserInterface() : m_iStateValue(0)
	{
		m_iObjectType = 300;
	}
	virtual ~HUserInterface();
};

class HButton : public HUserInterface
{
public:
	bool Frame();
	bool Render();

	HButton()
	{
		m_iObjectType = 301;
	}
};

