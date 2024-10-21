#pragma once

#include "Skill.h"

class KirbySkill : public Skill
{
private:
	const char* imageaddress;
public:
	KirbySkill()
	{
		Circle2D* c = new Circle2D(true, PMISSILE);
		SetObject(c);
	}
	~KirbySkill()
	{
		
	}
};