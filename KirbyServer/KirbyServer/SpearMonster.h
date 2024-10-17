#pragma once

#include "Monster.h"

class Monster;

class SpearMonster :public Monster
{
private:

public:
	SpearMonster()
		:Monster()
	{}

	SpearMonster(POINT p, EMonsterType mType, EMonsterState cs, POINT targetPos, int damage, int maxHealth, float speed, bool isEnabled)
		:Monster(p, mType, cs, targetPos, damage, maxHealth, speed, isEnabled)
	{}

	void Update() override;
};