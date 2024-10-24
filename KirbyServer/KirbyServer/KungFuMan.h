#pragma once

#include "Monster.h"

class Monster;

class KungFuMan :public Monster
{
private:
	float attackSpeed;

	std::chrono::high_resolution_clock::time_point t1_activate;
	std::chrono::high_resolution_clock::time_point t2_activate;
public:
	KungFuMan()
		:Monster(), attackSpeed(0.0f)
	{}

	KungFuMan(POINT p, EMonsterType mType, EMonsterState cs, POINT targetPos, int damage, int maxHealth, float speed, float attackSpeed, bool isEnabled)
		:Monster(p, mType, cs, targetPos, damage, maxHealth, speed, isEnabled), attackSpeed(attackSpeed)
	{}

	std::chrono::high_resolution_clock::time_point	Gettime_1() { return t1_activate; }
	std::chrono::high_resolution_clock::time_point	Gettime_2() { return t2_activate; }

	void Settime_1() { t1_activate = std::chrono::high_resolution_clock::now(); }
	void Settime_2() { t2_activate = std::chrono::high_resolution_clock::now(); }

	void StateUpdate() override;
	void Update() override;
};