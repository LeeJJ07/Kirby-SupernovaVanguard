#pragma once

#include "Skill.h"

class KunaiSkill : public Skill
{
private:
	const char* imageaddress;
public:
	KunaiSkill()
	{
		Rectangle2D* rectangle = new Rectangle2D(true, PMISSILE);
		SetObject(rectangle);
	}
	~KunaiSkill() {}
};