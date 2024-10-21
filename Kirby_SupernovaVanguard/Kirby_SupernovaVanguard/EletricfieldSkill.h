#pragma once

#include "Skill.h"

class ElectircfieldSkill : public Skill
{
private:
	const char* imageaddress;
public:
	ElectircfieldSkill()
	{
		Circle2D* c = new Circle2D(true, PMISSILE);
		SetObject(c);
	}
	~ElectircfieldSkill()
	{

	}
};