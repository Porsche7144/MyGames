#pragma once
#include "HNpcObject.h"

class HNpcAirObject : public HNpcObject
{
public:
	bool Frame() override;
	bool Render() override;
public:
	virtual bool Damage()				override;
	virtual bool Dead()					override;
	virtual void Attack(HObject* pPlayer)override;
};

