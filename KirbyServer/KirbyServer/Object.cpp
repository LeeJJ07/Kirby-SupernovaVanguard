#include "Object.h"
#include "UserData.h"

void Object::ObjectUpdate(UserData uD)
{
	SetPosition(uD.pos);
	GetCollider()->SetOffset(uD.offset);
	SetLookingDir(uD.lookingDir);
	SetMousePosition(uD.mousePos);
	SetIsInGame(uD.inGameStart);
}