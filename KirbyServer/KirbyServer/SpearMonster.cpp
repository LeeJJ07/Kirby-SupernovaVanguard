#include "SpearMonster.h"

void SpearMonster::Update()
{
	lookingDirection = NormalizationDir(targetPos);

	position.x += round(lookingDirection.first * speed);
	position.y += round(lookingDirection.second * speed);
}
