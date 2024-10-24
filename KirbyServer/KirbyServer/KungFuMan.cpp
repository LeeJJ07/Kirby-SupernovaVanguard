#include "KungFuMan.h"

void KungFuMan::StateUpdate()
{
	Settime_2();
	double changeTime = std::chrono::duration_cast<std::chrono::duration<double>>(Gettime_2() - Gettime_1()).count();
	
	switch (curState)
	{
	case CHASE:
		if (changeTime > KUNGFUMAN_CHASE_TIME)
		{
			curState = DETECT;
			Settime_1();
			Settime_2();
		}
		break;
	case ATTACK:
		if (changeTime > KUNGFUMAN_ATTACK_TIME)
		{
			curState = CHASE;
			Settime_1();
			Settime_2();
		}
		break;
	case DEATH:
		break;
	case DETECT:
		if (changeTime > KUNGFUMAN_DETECT_TIME)
		{
			curState = ATTACK;
			Settime_1();
			Settime_2();
		}
		break;
	}
}

void KungFuMan::Update()
{
	StateUpdate();
	switch (curState)
	{
	case CHASE:
		lookingDirection = NormalizationDir(targetPos);
		position.x += (int)round(lookingDirection.first * speed);
		position.y += (int)round(lookingDirection.second * speed);
		break;
	case ATTACK:
		lookingDirection = NormalizationDir(targetPos);
		position.x += (int)round(lookingDirection.first * attackSpeed);
		position.y += (int)round(lookingDirection.second * attackSpeed);
		break;
	case DEATH:
		break;
	case DETECT:
		lookingDirection = NormalizationDir(targetPos);
		break;
	}
}
