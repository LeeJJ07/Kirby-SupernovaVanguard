#include "LandMineMonster.h"

void LandMineMonster::Update()
{
	lookingDirection = NormalizationDir(targetPos);

	position.x += round(lookingDirection.first * speed);
	position.y += round(lookingDirection.second * speed);
}
