#pragma once

#include "Monster.h"

class Monster;

class FireManMonster :public Monster
{
private:
	float range;
public:
	FireManMonster()
		:Monster(),range(FIREMAN_BASE_RANGE)
	{}

	FireManMonster(POINT p, EMonsterType mType, EMonsterState cs, POINT targetPos, int damage, int maxHealth, float speed, bool isEnabled)
		:Monster(p, mType, cs, targetPos, damage, maxHealth, speed, isEnabled), range(FIREMAN_BASE_RANGE)
	{}

	bool IsInRange();

	void StateUpdate() override;
	void Update() override;
};