#pragma once

#include "Monster.h"

class RunnerMonster:public Monster
{
private:

public:
	RunnerMonster()
		:Monster()
	{}

	RunnerMonster(POINT p, EMonsterType mType, EMonsterState cs, POINT targetPos, int damage, int maxHealth, float speed, bool isEnabled)
		:Monster(p, mType, cs, targetPos, damage, maxHealth, speed, isEnabled)
	{}

	void StateUpdate() override;
	void Update() override;
};

