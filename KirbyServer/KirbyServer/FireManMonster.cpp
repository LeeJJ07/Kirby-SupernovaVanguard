#include "FireManMonster.h"

bool FireManMonster::IsInRange()
{
	int distance = (targetPos.x - position.x) * (targetPos.x - position.x) + (targetPos.y - position.y) * (targetPos.y - position.y);

	return distance <= range * range;
}

void FireManMonster::StateUpdate()
{
	switch (curState)
	{
	case CHASE:
		if (IsInRange())
			curState = ATTACK;
		break;
		break;
	case ATTACK:
		if (!IsInRange())
			curState = CHASE;
		break;
	case DEATH:
		break;
	}
}

void FireManMonster::Update()
{
	StateUpdate();
	if (curState == CHASE)
	{
		lookingDirection = NormalizationDir(targetPos);

		position.x += (int)round(lookingDirection.first * speed);
		position.y += (int)round(lookingDirection.second * speed);
	}
	else if (curState == ATTACK)
	{
		lookingDirection = NormalizationDir(targetPos);
	}
	else
	{

	}
}
