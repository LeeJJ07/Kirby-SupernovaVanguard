#pragma once


#include "Monster.h"

class Monster;

class GaoGao :public Monster
{
private:

public:
	GaoGao()
		:Monster()
	{}

	GaoGao(POINT p, EMonsterType mType, EMonsterState cs, int damage, int maxHealth, float speed, bool isEnabled)
		:Monster(p, mType, cs, damage, maxHealth, speed, isEnabled)
	{}

	void Update() override;
};