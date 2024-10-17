#include "Object.h"
#include "TotalData.h"

void Object::ObjectUpdate(PLAYERDATA uD)
{
	SetPosition(uD.pos);
	GetCollider()->SetOffset(uD.offset);
	SetLookingDir(uD.lookingDir);
	SetMousePosition(uD.mousePos);
	SetIsInGame(uD.inGameStart);
}

PAIR Object::NormalizationDir(POINT target)
{
	PAIR dir;
	double magnitude = sqrt((target.x - position.x) * (target.x - position.x) + (target.y - position.y) * (target.y - position.y));
	if (magnitude < 1e-3)
		return { 0, 0 };
	dir = { (target.x - position.x) / magnitude, (target.y - position.y) / magnitude };
	return dir;
}
