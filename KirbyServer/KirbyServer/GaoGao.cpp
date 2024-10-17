#include "GaoGao.h"

void GaoGao::Update()
{
	lookingDirection = NormalizationDir(targetPos);

	position.x += round(lookingDirection.first * speed);
	position.y += round(lookingDirection.second * speed);
}
