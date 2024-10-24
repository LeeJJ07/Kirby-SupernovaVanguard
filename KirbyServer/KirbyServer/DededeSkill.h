#pragma once

#include "Skill.h"

class DededeSkill : public Skill
{
private:
	float biggersize;

	Collider2D* collider;

	std::chrono::high_resolution_clock::time_point t1_activate;
	std::chrono::high_resolution_clock::time_point t2_activate;
public:
	DededeSkill(
		int masternum,
		int targetnum)
		: Skill(masternum, targetnum, SKILLTYPE::DEDEDESKILL, ECOLLIDERSHAPE::CIRCLE, 0, 20, 40, 0, 5., { 0,0 }, { totalData.udata[masternum].pos.x, totalData.udata[masternum].pos.y }, { 5,0 }),
		biggersize(2)
	{
		Circle2D* circle2D = new Circle2D(true, PMISSILE);
		circle2D->SetPosition(this->Getposition());
		SetCollider(circle2D);
	}
	~DededeSkill()
	{
		delete collider;
	}

	Collider2D* GetCollider() { return collider; }
	float		Getbiggersize() { return biggersize; }
	std::chrono::high_resolution_clock::time_point	Gettime_1() { return t1_activate; }
	std::chrono::high_resolution_clock::time_point	Gettime_2() { return t2_activate; }


	void SetCollider(Collider2D* collider) override { this->collider = collider; }
	void Setbiggersize(float biggersize) { this->biggersize = biggersize; }

	void DededeSkillBigger();
	void Settime_1() { t1_activate = std::chrono::high_resolution_clock::now(); }
	void Settime_2() { t2_activate = std::chrono::high_resolution_clock::now(); }
};

bool SetDededeSkillInDatasheet(Skill*& skill, int& ID)
{
	DededeSkill* dededeskill = dynamic_cast<DededeSkill*>(skill);
	Circle2D* dededecollider = dynamic_cast<Circle2D*>(dededeskill->GetCollider());

	totalData.sdata[ID].isActivate = dededeskill->Getisactivate();
	totalData.sdata[ID].id = dededeskill->GetID();
	totalData.sdata[ID].skillType = skill->Getskilltype();
	totalData.sdata[ID].size = skill->Getsize();
	totalData.sdata[ID].position = skill->Getposition();
	totalData.sdata[ID].colliderPosition = dededeskill->GetCollider()->GetPosition();
	totalData.sdata[ID].colliderSize = dededecollider->GetRadius();
	totalData.sdata[ID].colliderShape = skill->GetcolliderShape();
	totalData.sdata[ID].targetnum = skill->Gettargetnum();
	totalData.sdata[ID].dataType = SKILLTYPE;

	return true;
}

DededeSkill* dededeskill = nullptr;

void UpdateDededeSkill(Skill*& skill)
{
	dededeskill = dynamic_cast<DededeSkill*>(skill);

	POINT newpos;
	newpos.x = dededeskill->Getposition().x + dededeskill->Getdirection().x * dededeskill->Getspeed();
	newpos.y = dededeskill->Getposition().y + dededeskill->Getdirection().y * dededeskill->Getspeed();
	dededeskill->Setposition(newpos);

	dededeskill->DededeSkillBigger();

	dededeskill->GetCollider()->SetPosition(dededeskill->Getposition());
	Circle2D* circle = dynamic_cast<Circle2D*>(dededeskill->GetCollider());
	circle->SetRadius(dededeskill->Getsize());

	dededeskill->SetCollider(circle);

	dededeskill->Settime_2();
	double skilldestroytime = std::chrono::duration_cast<std::chrono::duration<double>>(dededeskill->Gettime_2() - dededeskill->Gettime_1()).count();
	if (skilldestroytime > TDEDEDESKILLDESTROY)
	{
		dededeskill->Setisactivate(false);
		OBJECTIDARR[dededeskill->GetID()] = false;
	}
}

void DededeSkill::DededeSkillBigger()
{
	this->Setsize(this->Getsize() + this->Getbiggersize());
}