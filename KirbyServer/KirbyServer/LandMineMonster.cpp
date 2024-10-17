#include "LandMineMonster.h"

void LandMineMonster::StateUpdate()
{
	switch (curState)
	{
	case CHASE:
		break;
	case DEATH:
		break;
	}
}

void LandMineMonster::Update()
{
	lookingDirection = NormalizationDir(targetPos);

	position.x += (int)round(lookingDirection.first * speed);
	position.y += (int)round(lookingDirection.second * speed);
}
