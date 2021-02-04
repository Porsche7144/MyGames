#pragma once
#include "Hcore.h"
#include "HInput.h"


class Sample : public Hcore
{
public:
	float    m_fNpcAppere;
	HInput	 m_Input;

public:
	bool	Init()		override;
	bool	Frame()		override;
	bool	Render()	override;
	bool	Release()	override;
public:
	Sample();
	virtual ~Sample();
};