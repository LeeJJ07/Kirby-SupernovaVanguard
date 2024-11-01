#pragma once

#include "Skill.h"

class MetaknightSkill : public Skill
{
public:
	MetaknightSkill(
		int masternum,
		int targetnum)
		: Skill(masternum, targetnum, SKILLTYPE::METAKNIGHTSKILL, ECOLLIDERSHAPE::RECTANGLE, 0, 10, 1000, 100, 80, 2., { 0,0 } , { totalData.udata[masternum].pos.x, totalData.udata[masternum].pos.y }, { 5,0 })
	{
		Rectangle2D* rectangle2D = new Rectangle2D(true, PMISSILE);
		rectangle2D->SetPosition(this->Getposition());
		SetCollider(rectangle2D);

		totalData.udata[Getmasternum()].curState = (ECharacterState)PATTACK;
	}
	~MetaknightSkill()
	{
		delete collider;
	}

	void	AssignSkill(int&, PLAYERDATA&, MONSTERDATA&);
};

void MetaknightSkill::AssignSkill(int& playerIndex, PLAYERDATA& playerData, MONSTERDATA& monsterData)
{
	PAIR lookingdir;
	if (!(vClient[playerIndex]->GetisLockOn()))
	{
		lookingdir = { (long)playerData.lookingDir.first, (long)playerData.lookingDir.second };
		Setdirection({ (long)lookingdir.first, (long)lookingdir.second });
	}
	else
	{
		lookingdir = { (Getposition().x - monsterData.pos.x), (Getposition().y - monsterData.pos.y) };

		if (lookingdir.first == 0)
			lookingdir.first = 0.1f;
		if (lookingdir.second == 0)
			lookingdir.second = 0.1f;

		double temp = sqrt(pow(lookingdir.first, 2) + pow(lookingdir.second, 2));
		lookingdir.first /= -temp / OFFSETADJUST; lookingdir.second /= -temp / OFFSETADJUST;

		Setdirection({ (long)lookingdir.first,(long)lookingdir.second });
	}
	Setangle(LookingDirToDegree(lookingdir));
	Setisactivate(true);
	Setoffset({ (long)playerData.lookingDir.first * (long)Getsize() / OFFSETADJUST / 2, (long)playerData.lookingDir.second * (long)Getsize() / OFFSETADJUST / 2 });
	Setposition({ playerData.pos.x + Getoffset().x, playerData.pos.y + Getoffset().y });
	Setmasternum(playerIndex);
}

static MetaknightSkill* metaknightskill = nullptr;

void UpdateMetaknightSkill(Skill*& skill)
{
	metaknightskill = dynamic_cast<MetaknightSkill*>(skill);

	POINT newpos;
	newpos.x = totalData.udata[metaknightskill->Getmasternum()].pos.x + metaknightskill->Getoffset().x;
	newpos.y = totalData.udata[metaknightskill->Getmasternum()].pos.y + metaknightskill->Getoffset().y;
	metaknightskill->Setposition(newpos);

	metaknightskill->GetCollider()->SetPosition(metaknightskill->Getposition());
	Rectangle2D* rectangle = dynamic_cast<Rectangle2D*>(metaknightskill->GetCollider());
	rectangle->SetWidth(metaknightskill->Getsize());
	rectangle->SetHeight(metaknightskill->Getsize2());

	metaknightskill->SetCollider(rectangle);

	metaknightskill->Sett2_destroy();
	double skilldestroytime = std::chrono::duration_cast<std::chrono::duration<double>>(metaknightskill->Gett2_destroy() - metaknightskill->Gett1_destroy()).count();

	metaknightskill->Sett2_attacktick();
	double hittime = std::chrono::duration_cast<std::chrono::duration<double>>(metaknightskill->Gett2_attacktick() - metaknightskill->Gett1_attacktick()).count();

	metaknightskill->Sett1_attacktick();

	if (totalData.udata[metaknightskill->Getmasternum()].curState == (ECharacterState)PATTACK
		&& skilldestroytime > METAKNIGHT_SKILL_END_TIME)
	{
		totalData.udata[metaknightskill->Getmasternum()].curState = (ECharacterState)PIDLE;
	}

	if (skilldestroytime > 0.5)
	{
		OBJECTIDARR[metaknightskill->GetID()] = false;
		delete skill;
		skill = nullptr;
	}
}