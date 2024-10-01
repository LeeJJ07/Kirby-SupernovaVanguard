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
	if (!p[ud.id])
		p[ud.id] = new Player();
	
	p[ud.id]->SetCenter(ud.center);
	p[ud.id]->SetLookingDir(ud.lookingDir);
	p[ud.id]->SetMoveDir(ud.moveDir);
	p[ud.id]->SetPos(ud.pos);
	p[ud.id]->SetRadius(ud.radius);
}

void SetPlayer(Player* &p, UserData& ud)
{
	if (!p)
		p = new Player();

	p->SetCenter(ud.center);
	p->SetLookingDir(ud.lookingDir);
	p->SetMoveDir(ud.moveDir);
	p->SetPos(ud.pos);
	p->SetRadius(ud.radius);
}