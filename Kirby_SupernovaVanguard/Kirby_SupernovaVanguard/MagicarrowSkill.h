#pragma once

#include "Skill.h"

class MagicarrowSkill : public Skill
{
private:
	const char* imageaddress;
public:
	MagicarrowSkill()
	{
		Rectangle2D* rectangle = new Rectangle2D(true, PMISSILE);
		SetObject(rectangle);
	}
	~MagicarrowSkill() {}
};