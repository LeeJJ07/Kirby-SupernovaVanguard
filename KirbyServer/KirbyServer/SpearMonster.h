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

	SpearMonster(POINT p, EMonsterType mType, EMonsterState cs, int damage, int maxHealth, float speed, bool isEnabled)
		:Monster(p, mType, cs, damage, maxHealth, speed, isEnabled)
	{}

	void Update() override;
};