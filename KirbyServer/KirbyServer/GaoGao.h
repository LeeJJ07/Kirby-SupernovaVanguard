#pragma once


#include "Monster.h"

class Monster;

class GaoGao :public Monster
{
private:
	POINT curTargetPos;

	std::chrono::high_resolution_clock::time_point t1_activate;
	std::chrono::high_resolution_clock::time_point t2_activate;
public:
	GaoGao()
		:Monster(), curTargetPos({ 0, 0 })
	{}

	GaoGao(POINT p, int radius, EMonsterType mType, EMonsterState cs, POINT targetPos, int damage, int maxHealth, float speed, bool isEnabled)
		:Monster(p, radius, mType, cs, targetPos, damage, maxHealth, speed, isEnabled), curTargetPos({ 0, 0 })
	{}

	std::chrono::high_resolution_clock::time_point	Gettime_1() { return t1_activate; }
	std::chrono::high_resolution_clock::time_point	Gettime_2() { return t2_activate; }

	void Settime_1() { t1_activate = std::chrono::high_resolution_clock::now(); }
	void Settime_2() { t2_activate = std::chrono::high_resolution_clock::now(); }

	void StateUpdate() override;
	void Update() override;
};