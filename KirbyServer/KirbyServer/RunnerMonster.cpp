#include "RunnerMonster.h"

void RunnerMonster::StateUpdate()
{
	switch (curState)
	{
	case CHASE:
		break;
	case DEATH:
		break;
	}
}

void RunnerMonster::Update()
{
	lookingDirection = NormalizationDir(targetPos);

	position.x += (int)round(lookingDirection.first * speed);
	position.y += (int)round(lookingDirection.second * speed);
}
