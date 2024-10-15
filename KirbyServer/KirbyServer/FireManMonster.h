#pragma once

#include "Monster.h"

class Monster;

class FireManMonster :public Monster
{
private:

public:
	FireManMonster()
		:Monster()
	{}

	FireManMonster(POINT p, EMonsterType mType, EMonsterState cs, int damage, int maxHealth, float speed, bool isEnabled)
		:Monster(p, mType, cs, damage, maxHealth, speed, isEnabled)
	{}

	void Update() override;
};