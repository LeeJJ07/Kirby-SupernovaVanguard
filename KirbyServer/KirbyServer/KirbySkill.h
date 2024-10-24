#pragma once

#include "Skill.h"

#define KIRBYSKILLMAXXSIZE 50

class KirbySkill : public Skill
{
private:
	float biggersize;
	float decelerationrate;

	Collider2D* collider;

	std::chrono::high_resolution_clock::time_point t1_activate;
	std::chrono::high_resolution_clock::time_point t2_activate;
public:
	KirbySkill(
		int masternum,
		int targetnum)
		: Skill(masternum, targetnum, SKILLTYPE::KIRBYSKILL, ECOLLIDERSHAPE::CIRCLE, 0.1, 10, 5, 0, 3., { 0,0 }, { totalData.udata[masternum].pos.x, totalData.udata[masternum].pos.y }, { 5,0 }),
		biggersize(1),
		decelerationrate(0.01)
	{
		Circle2D* circle2D = new Circle2D(true, PMISSILE);
		circle2D->SetPosition(this->Getposition());
		SetCollider(circle2D);
	}
	~KirbySkill()
	{
		/*delete imageaddress;
		delete collider;*/
	}

	Collider2D* GetCollider()	{ return collider; }
	float		Getbiggersize()	{ return biggersize; }
	float		Getdecelerationrate()	{ return decelerationrate; }
	std::chrono::high_resolution_clock::time_point	Gettime_1() { return t1_activate; }
	std::chrono::high_resolution_clock::time_point	Gettime_2() { return t2_activate; }


	void SetCollider(Collider2D* collider) override		{ this->collider = collider; }
	void Setbiggersize(float biggersize)	{ this->biggersize = biggersize; }
	void Setdecelerationrate(float decelerationrate)	{ this->decelerationrate = decelerationrate; }
	void Settime_1() { t1_activate = std::chrono::high_resolution_clock::now(); }
	void Settime_2() { t2_activate = std::chrono::high_resolution_clock::now(); }

	void KirbySkillBigger();
	void KirbySkillSlower();
};

static KirbySkill* kirbyskill = nullptr;

bool SetKirbySkillInDatasheet(Skill*& skill, int& ID)
{
	kirbyskill = dynamic_cast<KirbySkill*>(skill);
	Circle2D*	kirbycollider = dynamic_cast<Circle2D*>(kirbyskill->GetCollider());

	totalData.sdata[ID].isActivate = kirbyskill->Getisactivate();
	totalData.sdata[ID].id = kirbyskill->GetID();
	totalData.sdata[ID].skillType = skill->Getskilltype();
	totalData.sdata[ID].size = skill->Getsize();
	totalData.sdata[ID].position = skill->Getposition();
	totalData.sdata[ID].colliderPosition = kirbyskill->GetCollider()->GetPosition();
	totalData.sdata[ID].colliderSize = kirbycollider->GetRadius();
	totalData.sdata[ID].colliderShape = skill->GetcolliderShape();
	totalData.sdata[ID].targetnum = skill->Gettargetnum();
	totalData.sdata[ID].dataType = SKILLTYPE;

	return true;
}


void UpdateKirbySkill(Skill* &skill)
{
	kirbyskill = dynamic_cast<KirbySkill*>(skill);

	POINT newpos;
	newpos.x = kirbyskill->Getposition().x + kirbyskill->Getdirection().x * kirbyskill->Getspeed();
	newpos.y = kirbyskill->Getposition().y + kirbyskill->Getdirection().y * kirbyskill->Getspeed();
	kirbyskill->Setposition(newpos);

	if (kirbyskill->Getsize() < KIRBYSKILLMAXXSIZE)
		kirbyskill->KirbySkillBigger();
	else if (kirbyskill->Getspeed() > 0)
		kirbyskill->KirbySkillSlower();

	kirbyskill->GetCollider()->SetPosition(kirbyskill->Getposition());
	Circle2D* circle = dynamic_cast<Circle2D*>(kirbyskill->GetCollider());
	circle->SetRadius(kirbyskill->Getsize());
	
	kirbyskill->SetCollider(circle);

	kirbyskill->Settime_2();
	double skilldestroytime = std::chrono::duration_cast<std::chrono::duration<double>>(kirbyskill->Gettime_2() - kirbyskill->Gettime_1()).count();
	if (skilldestroytime > TKIRBYSKILLDESTROY)
	{
		kirbyskill->Setisactivate(false);
		OBJECTIDARR[kirbyskill->GetID()] = false;
	}
}

void KirbySkill::KirbySkillBigger()
{
	this->Setsize(this->Getsize() + this->Getbiggersize());
}

void KirbySkill::KirbySkillSlower()
{
	this->Setspeed(this->Getspeed() - this->Getdecelerationrate());
	if (this->Getspeed() <= 0)
		this->Setspeed(0);
}