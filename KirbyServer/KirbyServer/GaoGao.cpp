#include "GaoGao.h"

void GaoGao::StateUpdate()
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

void GaoGao::Update()
{
	lookingDirection = NormalizationDir(targetPos);

	position.x += (int)round(lookingDirection.first * speed);
	position.y += (int)round(lookingDirection.second * speed);
}
