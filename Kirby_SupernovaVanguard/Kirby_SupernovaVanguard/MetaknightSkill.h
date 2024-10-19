#pragma once

#include "Skill.h"

class MetaknightSkill : public Skill
{
private:
	const char* imageaddress;
public:
	MetaknightSkill()
	{
		Rectangle2D* rectangle = new Rectangle2D(true, PMISSILE);
		SetObject(rectangle);
	}
	~MetaknightSkill() {}
};