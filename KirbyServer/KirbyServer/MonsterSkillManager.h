#pragma once

#include "framework.h"

class MonsterSkill;

class MonsterSkillManager
{
private:
	std::vector<MonsterSkill*> skillVector;
public:
	MonsterSkillManager()
	{}
	~MonsterSkillManager()
	{
		for (auto value : skillVector)
			delete value;
	}

	std::vector<MonsterSkill*>& GetskillVector() { return skillVector; }
};