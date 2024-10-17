#include "RunnerMonster.h"

void RunnerMonster::Update()
{
	// 수정 필요
	lookingDirection = NormalizationDir(targetPos);

	position.x += round(lookingDirection.first * speed);
	position.y += round(lookingDirection.second * speed);
}
