#pragma once

#include "Skill.h"
#include "Monster.h"

class TruckSkill : public Skill
{
private:
	const char* imageaddress;
public:
	TruckSkill()
	{
		Rectangle2D* rectangle = new Rectangle2D(true, PMISSILE);
		SetObject(rectangle);
	}
	~TruckSkill() {}
};