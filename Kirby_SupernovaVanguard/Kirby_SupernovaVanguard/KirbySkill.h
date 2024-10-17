#pragma once

#include "Skill.h"

class KirbySkill : public Skill
{
private:
	float biggersize;
	float decelerationrate;
	const char* imageaddress;
public:
	KirbySkill()
	{
		Circle2D* c = new Circle2D(true, PMISSILE);
		SetObject(c);
	}
	~KirbySkill();
};