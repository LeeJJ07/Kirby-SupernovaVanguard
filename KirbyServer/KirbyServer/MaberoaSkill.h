#pragma once

#include "Skill.h"

#define MABEOROASKILLMAXXSIZE 20

class MaberoaSkill : public Skill
{
private:
	float biggersize;
public:
	MaberoaSkill(
		int masternum,
		int targetnum,
		int radius)
		: Skill(masternum, targetnum, SKILLTYPE::MABEROASKILL, ECOLLIDERSHAPE::CIRCLE, 0.2, 5, 1000, 0, 0, 2., { 0,0 }, { totalData.udata[masternum].pos.x, totalData.udata[masternum].pos.y }, { 5,0 }),
		biggersize(1)
	{
		Circle2D* circle2D = new Circle2D(true, PMISSILE, radius);
		circle2D->SetPosition(this->Getposition());
		SetCollider(circle2D);
		Sett1_destroy();

	}
	~MaberoaSkill()
	{
		delete collider;
	}

	Collider2D* GetCollider() { return collider; }
	float		Getbiggersize() { return biggersize; }

	void Setbiggersize(float biggersize) { this->biggersize = biggersize; }

	void MaberoaSkillBigger();

	void AssignSkill(int& , PLAYERDATA& , MONSTERDATA& );
};

void MaberoaSkill::AssignSkill(int& playerIndex, PLAYERDATA& playerData, MONSTERDATA& monsterData)
{
	static int num = 0;
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
	switch (GetAmount())
	{
	case 1:
		Setoffset({ 0, -20 });
		break;
	case 2:
		Setoffset({ num * 30 - 15 * (GetAmount() - 1),-20 });
		break;
	case 3:
		Setoffset({ num * 30 - 15 * (GetAmount() - 1),-20 });
		break;
	}
	Setposition({ playerData.pos.x + Getoffset().x, playerData.pos.y + Getoffset().y });
	Setmasternum(playerIndex);
	num++;
	if (GetAmount() == num)
		num = 0;
}

static MaberoaSkill* maberoaskill = nullptr;

void UpdateMaberoaSkill(Skill*& skill)
{
	maberoaskill = dynamic_cast<MaberoaSkill*>(skill);

	if (maberoaskill->Getsize() < MABEOROASKILLMAXXSIZE)
	{
		maberoaskill->MaberoaSkillBigger();
		int x = totalData.udata[maberoaskill->Getmasternum()].pos.x + maberoaskill->Getoffset().x;
		int y = totalData.udata[maberoaskill->Getmasternum()].pos.y + maberoaskill->Getoffset().y;
		maberoaskill->Setposition({ x,y });
	}
	else
	{
		POINT newpos;
		newpos.x = maberoaskill->Getposition().x + maberoaskill->Getdirection().x * maberoaskill->Getspeed();
		newpos.y = maberoaskill->Getposition().y + maberoaskill->Getdirection().y * maberoaskill->Getspeed();
		maberoaskill->Setposition(newpos);
	}

	skill->GetCollider()->SetPosition(maberoaskill->Getposition());
	Circle2D* circle = dynamic_cast<Circle2D*>(skill->GetCollider());
	circle->SetRadius(skill->Getsize());

	skill->SetCollider(circle);

	skill->Sett2_destroy();
	double skilldestroytime = std::chrono::duration_cast<std::chrono::duration<double>>(skill->Gett2_destroy() - skill->Gett1_destroy()).count();

	skill->Sett2_attacktick();
	double hittime = std::chrono::duration_cast<std::chrono::duration<double>>(skill->Gett2_attacktick() - skill->Gett1_attacktick()).count();

	skill->Sett1_attacktick();

	if (skilldestroytime > TKIRBYSKILLDESTROY)
	{
		OBJECTIDARR[skill->GetID()] = false;
		delete skill;
		skill = nullptr;
	}
}

void MaberoaSkill::MaberoaSkillBigger()
{
	this->Setsize(this->Getsize() + this->Getbiggersize());
}