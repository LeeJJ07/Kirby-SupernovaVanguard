#pragma once

#include "Skill.h"

#define KIRBYSKILLMAXXSIZE 15

class MaberoaSkill : public Skill
{
private:
	float biggersize;
	const char* imageaddress;

	Collider2D* collider;

	std::chrono::high_resolution_clock::time_point t1_activate;
	std::chrono::high_resolution_clock::time_point t2_activate;
public:
	MaberoaSkill(
		int masternum,
		int targetnum)
		: Skill(masternum, targetnum, SKILLTYPE::MABEROASKILL, COLLIDERTYPE::CIRCLE, 0.2, 5, 0, 0, 2., { 0,0 }, { totalData.udata[masternum].pos.x, totalData.udata[masternum].pos.y }, { 5,0 }),
		imageaddress(nullptr), biggersize(1)
	{
		Circle2D* circle2D = new Circle2D(true, PMISSILE);
		circle2D->SetPosition(this->Getposition());
		SetCollider(circle2D);
	}
	~MaberoaSkill()
	{
		delete imageaddress;
		delete collider;
	}

	Collider2D* GetCollider() { return collider; }
	float		Getbiggersize() { return biggersize; }
	std::chrono::high_resolution_clock::time_point	Gettime_1() { return t1_activate; }
	std::chrono::high_resolution_clock::time_point	Gettime_2() { return t2_activate; }

	void SetCollider(Collider2D* collider) override { this->collider = collider; }
	void Setbiggersize(float biggersize) { this->biggersize = biggersize; }

	void Settime_1() { t1_activate = std::chrono::high_resolution_clock::now(); }
	void Settime_2() { t2_activate = std::chrono::high_resolution_clock::now(); }

	void MaberoaSkillBigger();
};

bool SetMaberoaSkillInDatasheet(Skill*& skill, int& ID)
{
	MaberoaSkill* maberoaskill = dynamic_cast<MaberoaSkill*>(skill);
	Circle2D* maberoacollider = dynamic_cast<Circle2D*>(maberoaskill->GetCollider());

	totalData.sdata[ID].isactivate = maberoaskill->Getisactivate();
	totalData.sdata[ID].id = maberoaskill->GetID();
	totalData.sdata[ID].skilltype = skill->Getskilltype();
	totalData.sdata[ID].size = skill->Getsize();
	totalData.sdata[ID].position = skill->Getposition();
	totalData.sdata[ID].colliderposition = maberoaskill->GetCollider()->GetPosition();
	totalData.sdata[ID].collidersize = maberoacollider->GetRadius();
	totalData.sdata[ID].collidertype = skill->Getcollidertype();
	totalData.sdata[ID].targetnum = skill->Gettargetnum();

	return true;
}

MaberoaSkill* maberoaskill = nullptr;

void UpdateMaberoaSkill(Skill*& skill)
{
	maberoaskill = dynamic_cast<MaberoaSkill*>(skill);

	if (maberoaskill->Getsize() < KIRBYSKILLMAXXSIZE)
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

	maberoaskill->GetCollider()->SetPosition(maberoaskill->Getposition());
	Circle2D* circle = dynamic_cast<Circle2D*>(maberoaskill->GetCollider());
	circle->SetRadius(maberoaskill->Getsize());

	maberoaskill->SetCollider(circle);

	maberoaskill->Settime_2();
	double skilldestroytime = std::chrono::duration_cast<std::chrono::duration<double>>(maberoaskill->Gettime_2() - maberoaskill->Gettime_1()).count();
	if (skilldestroytime > TKIRBYSKILLDESTROY)
	{
		maberoaskill->Setisactivate(false);
		OBJECTIDARR[maberoaskill->GetID()] = false;
	}
}

void MaberoaSkill::MaberoaSkillBigger()
{
	this->Setsize(this->Getsize() + this->Getbiggersize());
}