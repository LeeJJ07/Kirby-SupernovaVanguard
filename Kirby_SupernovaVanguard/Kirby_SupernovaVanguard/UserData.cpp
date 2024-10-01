#include "UserData.h"

void SetUserData(UserData& ud, Player* p)
{
	ud.center = p->GetCenter();
	ud.lookingDir = p->GetLookingDir();
	ud.moveDir = p->GetMoveDir();
	ud.pos = p->GetPos();
	ud.radius = p->GetRadius();
}

void SetPlayer(Player* p, UserData& ud)
{
	p->SetCenter(ud.center);
	p->SetLookingDir(ud.lookingDir);
	p->SetMoveDir(ud.moveDir);
	p->SetPos(ud.pos);
	p->SetRadius(ud.radius);
}