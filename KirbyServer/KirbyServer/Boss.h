#pragma once

#include "Monster.h"

#define PHASECHANGETIME 8
#define PHASENUM 2

extern int BossID;

class Monster;

enum BossPhase
{
	Electric,
	Flame,
};

class Boss: public Monster
{
private:
	POINT curTargetPos;
	int phase = 0;

	std::chrono::high_resolution_clock::time_point t1_phaseSwitch;
	std::chrono::high_resolution_clock::time_point t2_phaseSwitch;
public:
	Boss()
		:Monster(), curTargetPos({ 0, 0 })
	{}

	Boss(POINT p, int radius, EMonsterType mType, EMonsterState cs, POINT targetPos, int damage, int maxHealth, float speed, bool isEnabled)
		:Monster(p, radius, mType, cs, targetPos, damage, maxHealth, speed, isEnabled), curTargetPos(targetPos)
	{}

	std::chrono::high_resolution_clock::time_point	Gettime_1() { return t1_phaseSwitch; }
	std::chrono::high_resolution_clock::time_point	Gettime_2() { return t2_phaseSwitch; }

	void Settime_1() { t1_phaseSwitch = std::chrono::high_resolution_clock::now(); }
	void Settime_2() { t2_phaseSwitch = std::chrono::high_resolution_clock::now(); }

public:
	void StateUpdate() override;
	void Update() override;
};

void InitLasorSkill(Monster*&);
void InitFireballSkill(Monster*&);