#pragma once

#include "framework.h"

class Skill;

class SkillManager
{
private:
	std::vector<Skill*> skillVector;
public:
	SkillManager()
	{}
	~SkillManager()
	{
		for (auto value : skillVector)
			delete value;
	}

	std::vector<Skill*>& GetskillVector()	{ return skillVector; }
};