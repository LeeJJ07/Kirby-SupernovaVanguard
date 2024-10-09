#include "UserData.h"

void SetUserData(UserData& ud, Object* p)
{
	ud.pos = p->GetPosition();
	ud.offset = p->GetCollider()->GetOffset();
	ud.lookingDir = p->GetLookingDir();
	ud.mousePos = p->GetMousePosition();

	int nRadius = ((Circle2D*)((Player*)p->GetCollider()))->GetRadius();

	if (nRadius > 0)
		ud.radius = nRadius;
}

void SetObject(Object* &p, UserData& ud)
{
	if (!p)
		p = new Player();

	p->SetPosition(ud.pos);
	p->SetLookingDir(ud.lookingDir);
	p->SetMousePosition(ud.mousePos);
	p->GetCollider()->SetOffset(ud.offset);
	p->SetIsInGame(ud.inGameStart);
}