#include "Skill.h"

void Skill::SetSkillInDataSheet(SKILLDATA& skillData)
{
	switch (GetcolliderShape())
	{
	case CIRCLE:
	{
		Circle2D* circleCollider = dynamic_cast<Circle2D*>(GetCollider());
		skillData.colliderSize = circleCollider->GetRadius();
	}
		break;
	case RECTANGLE:
	{
		Rectangle2D* rectangleCollider = dynamic_cast<Rectangle2D*>(GetCollider());
		skillData.colliderSize = rectangleCollider->GetWidth();
		skillData.colliderSize2 = rectangleCollider->GetHeight();
	}
		break;
	}

	skillData.isActivate = Getisactivate();
	skillData.id = GetID();
	skillData.skillType = Getskilltype();
	skillData.size = Getsize();
	skillData.size2 = Getsize2();
	skillData.position = Getposition();
	skillData.colliderPosition = GetCollider()->GetPosition();
	skillData.colliderShape = GetcolliderShape();
	skillData.targetnum = Gettargetnum();
	skillData.angle = Getangle();
	skillData.dataType = SKILLTYPE;
}

void Skill::AssignSkill(int& playerIndex, PLAYERDATA& playerData, MONSTERDATA& monsterData)
{
	if (!(vClient[playerIndex]->GetisLockOn()))
		Setdirection({ (long)playerData.lookingDir.first, (long)playerData.lookingDir.second });
	else
	{
		PAIR lookingdir = { (Getposition().x - monsterData.pos.x), (Getposition().y - monsterData.pos.y) };
		double temp = sqrt(pow(lookingdir.first, 2) + pow(lookingdir.second, 2));
		lookingdir.first /= temp / OFFSETADJUST; lookingdir.second /= temp / OFFSETADJUST;

		Setdirection({ (long)(-lookingdir.first),(long)(-lookingdir.second) });
	}
	Setisactivate(true);
	Setoffset({ (long)playerData.lookingDir.first * (long)Getsize() / OFFSETADJUST / 2,
		(long)playerData.lookingDir.second * (long)Getsize() / OFFSETADJUST / 2 });
	Setposition({ playerData.pos.x + Getoffset().x, playerData.pos.y + Getoffset().y });
	Setmasternum(playerIndex);
}