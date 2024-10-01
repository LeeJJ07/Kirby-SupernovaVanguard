#include "UserData.h"

void SetUserData(UserData& ud, Player* p)
{
	ud.center = p->GetCenter();
	ud.lookingDir = p->GetLookingDir();
	ud.moveDir = p->GetMoveDir();
	ud.pos = p->GetPos();
	ud.radius = p->GetRadius();
}