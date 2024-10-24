#include "GaoGao.h"

void GaoGao::StateUpdate()
{
	double changeTime = 0;
	if(curState == DETECT || curState==STUN)
	{
		Settime_2();
		changeTime = std::chrono::duration_cast<std::chrono::duration<double>>(Gettime_2() - Gettime_1()).count();
	}

	switch (curState)
	{
	case ATTACK:
		if (abs(curTargetPos.x - position.x) < 15 
			&& abs(curTargetPos.y - position.y) < 15)
		{
			curState = STUN;
			Settime_1();
			Settime_2();
		}
		break;
	case DEATH:
		break;
	case DETECT:
		if (changeTime > 2.0f)
			curState = ATTACK;
		break;
	case STUN:
		if (changeTime > 3.0f)
		{
			curState = DETECT;
			Settime_1();
			Settime_2();
		}
		break;
	}
}

void GaoGao::Update()
{
	StateUpdate();
	switch (curState)
	{
	case ATTACK:
		lookingDirection = NormalizationDir(curTargetPos);
		position.x += (int)round(lookingDirection.first * speed);
		position.y += (int)round(lookingDirection.second * speed);
		break;
	case DEATH:
		break;
	case DETECT:
		lookingDirection = NormalizationDir(targetPos);
		curTargetPos = targetPos;
		curTargetPos.x += lookingDirection.first * 150;
		curTargetPos.y += lookingDirection.second * 150;
		break;
	case STUN:
		break;
	}
}
