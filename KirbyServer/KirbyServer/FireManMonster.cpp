#include "FireManMonster.h"

void FireManMonster::StateUpdate()
{
	switch (curState)
	{
	case CHASE:
		break;
	case DEATH:
		break;
	}
}

void FireManMonster::Update()
{
	lookingDirection = NormalizationDir(targetPos);

	position.x += (int)round(lookingDirection.first * speed);
	position.y += (int)round(lookingDirection.second * speed);
}
