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

	void SetCollider(Collider2D* collider) override { this->collider = collider; }
	void Setbiggersize(float biggersize) { this->biggersize = biggersize; }

	void MaberoaSkillBigger();
};

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

	if (hittime > MABEROATICK)
	{
		skill->Setcanhit(true);
		skill->Sett1_attacktick();
	}
	else
	{
		skill->Setcanhit(false);
	}

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