#include "KungFuMan.h"

void KungFuMan::StateUpdate()
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

void KungFuMan::Update()
{
	lookingDirection = NormalizationDir(targetPos);

	position.x += (int)round(lookingDirection.first * speed);
	position.y += (int)round(lookingDirection.second * speed);
}
