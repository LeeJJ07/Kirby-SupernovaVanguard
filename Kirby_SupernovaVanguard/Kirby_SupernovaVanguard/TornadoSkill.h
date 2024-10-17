#pragma once

#include "Skill.h"
#include "Monster.h"

class TornadoSkill : public Skill
{
private:
	int tick;
	bool isleftright;
	const char* imageaddress;
	Object* target;
};