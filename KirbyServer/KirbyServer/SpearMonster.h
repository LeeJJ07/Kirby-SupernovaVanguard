#pragma once

#include "Monster.h"

class Monster;

class SpearMonster :public Monster
{
private:
	float range;
public:
	SpearMonster()
		:Monster(), range(SPEAR_BASE_RANGE)
	{}

	SpearMonster(POINT p, int radius, EMonsterType mType, EMonsterState cs, POINT targetPos, int damage, int maxHealth, float speed, float r, bool isEnabled)
		:Monster(p, radius, mType, cs, targetPos, damage, maxHealth, speed, isEnabled), range(r)
	{}

	bool IsInRange();

	void StateUpdate() override;
	void Update() override;
};