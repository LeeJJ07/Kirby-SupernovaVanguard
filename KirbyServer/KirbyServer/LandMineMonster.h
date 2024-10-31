#pragma once

#include "Monster.h"

class LandMineMonster :public Monster
{
private:

public:
	LandMineMonster()
		:Monster()
	{}

	LandMineMonster(POINT p, int radius, EMonsterType mType, EMonsterState cs, POINT targetPos, int damage, int maxHealth, float speed, bool isEnabled)
		:Monster(p, radius, mType, cs, targetPos, damage, maxHealth, speed, isEnabled)
	{}

	void StateUpdate() override;
	void Update() override;
};