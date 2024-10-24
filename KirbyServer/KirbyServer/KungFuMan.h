#pragma once

#include "Monster.h"

class Monster;

class KungFuMan :public Monster
{
private:
	float attackSpeed;
public:
	KungFuMan()
		:Monster(), attackSpeed(0.0f)
	{}

	KungFuMan(POINT p, EMonsterType mType, EMonsterState cs, POINT targetPos, int damage, int maxHealth, float speed, float attackSpeed, bool isEnabled)
		:Monster(p, mType, cs, targetPos, damage, maxHealth, speed, isEnabled), attackSpeed(attackSpeed)
	{}

	void StateUpdate() override;
	void Update() override;
};