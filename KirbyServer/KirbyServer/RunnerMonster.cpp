#include "RunnerMonster.h"

void RunnerMonster::Update()
{
	// ���� �ʿ�
	lookingDirection = NormalizationDir(targetPos);

	position.x += round(lookingDirection.first * speed);
	position.y += round(lookingDirection.second * speed);
}
