#pragma once

#include "Skill.h"

class MaberoaSkill : public Skill
{
private:
	const char* imageaddress;
public:
	MaberoaSkill()
	{
		Circle2D* circle = new Circle2D(true, PMISSILE);
		SetObject(circle);
	}
	~MaberoaSkill() {}
};