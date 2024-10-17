#include "WingBugMonster.h"

void WingBugMonster::Update()
{
	lookingDirection = NormalizationDir(targetPos);

	position.x += round(lookingDirection.first * speed);
	position.y += round(lookingDirection.second * speed);
}
