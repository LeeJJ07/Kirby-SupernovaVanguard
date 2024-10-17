#pragma once

#include "Monster.h"

class LandMineMonster :public Monster
{
private:

public:
	LandMineMonster()
		:Monster()
	{}

	LandMineMonster(POINT p, EMonsterType mType, EMonsterState cs, POINT targetPos, int damage, int maxHealth, float speed, bool isEnabled)
		:Monster(p, mType, cs, targetPos, damage, maxHealth, speed, isEnabled)
	{}

	void Update() override;

};