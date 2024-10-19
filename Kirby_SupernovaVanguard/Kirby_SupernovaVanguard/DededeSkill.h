#pragma once

#include "Skill.h"

class DededeSkill : public Skill
{
private:
	const char* imageaddress;
public:
	DededeSkill()
	{
		Circle2D* circle = new Circle2D(true, PMISSILE);
		SetObject(circle);
	}
	~DededeSkill() {}
};