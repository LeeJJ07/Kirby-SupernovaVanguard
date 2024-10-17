#include "Boss.h"

void Boss::StateUpdate()
{
	switch (curState)
	{
	case CHASE:
		break;
	case ATTACK:
		break;
	case DEATH:
		break;
	}
}

void Boss::Update()
{
	lookingDirection = NormalizationDir(targetPos);

	position.x += (int)round(lookingDirection.first * speed);
	position.y += (int)round(lookingDirection.second * speed);
}