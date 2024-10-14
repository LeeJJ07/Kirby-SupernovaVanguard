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