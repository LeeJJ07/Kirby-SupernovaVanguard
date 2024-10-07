#include "UserData.h"
#include "Map.h"
#include "Camera.h"

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
	{
		p[ud.id] = new Player();
		Create(p[ud.id]);
	}
	
	p[ud.id]->SetCenter(ud.center);
	p[ud.id]->SetLookingDir(ud.lookingDir);
	p[ud.id]->SetMoveDir(ud.moveDir);
	p[ud.id]->SetSelectNumber(GetCharacterIndex(ud.mousePos));
	p[ud.id]->SetPos(ud.pos);
	p[ud.id]->SetRadius(ud.radius);

	camera.PositionUpdate();
}

void SetPlayer(Player* &p, UserData& ud)
{
	if (!p)
		p = new Player();

	POINT temp = { p->GetCenter().x + ud.center.x,p->GetCenter().y + ud.center.y };

	p->SetCenter(ud.center);
	p->SetLookingDir(ud.lookingDir);
	p->SetMoveDir(ud.moveDir);
	p->SetSelectNumber(GetCharacterIndex(ud.mousePos));
	p->SetPos(temp);
	p->SetRadius(ud.radius);
}

int GetCharacterIndex(POINT mousePos)
{
	return 0;
}