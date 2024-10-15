#pragma once

#include "Monster.h"

class RunnerMonster:public Monster
{
private:

public:
	RunnerMonster()
		:Monster()
	{}

	RunnerMonster(POINT p, EMonsterType mType, EMonsterState cs, int damage, int maxHealth, float speed, bool isEnabled)
		:Monster(p, mType, cs, damage, maxHealth, speed, isEnabled)
	{}

	void Update() override;

};

