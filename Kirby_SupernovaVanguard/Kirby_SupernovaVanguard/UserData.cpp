#include "UserData.h"

void SetUserData(UserData& ud, Player* p)
{
	ud.center = p->GetCenter();
	ud.lookingDir = p->GetLookingDir();
	ud.moveDir = p->GetMoveDir();
	ud.pos = p->GetPos();
	ud.radius = p->GetRadius();
}

void SetPlayer(std::vector<Player*>& p, UserData& ud)
{
		p[ud.id]->SetCenter(ud.center);
		p[ud.id]->SetLookingDir(ud.lookingDir);
		p[ud.id]->SetMoveDir(ud.moveDir);
		p[ud.id]->SetPos(ud.pos);
		p[ud.id]->SetRadius(ud.radius);
}