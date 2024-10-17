#include "WingBugMonster.h"

void WingBugMonster::StateUpdate()
{
	switch (curState)
	{
	case CHASE:
		break;
	case DEATH:
		break;
	}
}
void WingBugMonster::Update()
{
	if ((position.x < -50 && lookingDirection.first < 0)
		|| (position.x > MAX_MAP_SIZE_X + 50 && lookingDirection.first > 0))
	{
		this->SetEnabled(false);
		return;
	}
	position.x += (int)round(lookingDirection.first * speed);
}
