#pragma once
#include "Monster.h"

class Monster;

class WingBugMonster :public Monster
{
private:

public:
	WingBugMonster()
		:Monster()
	{}

	WingBugMonster(POINT p, EMonsterType mType, EMonsterState cs, int damage, int maxHealth, float speed, bool isEnabled)
		:Monster(p, mType, cs, damage, maxHealth, speed, isEnabled)
	{}

	void Update() override;
};