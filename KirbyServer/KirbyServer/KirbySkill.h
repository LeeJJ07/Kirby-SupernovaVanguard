#pragma once

#include "Skill.h"
#include "TotalData.h"

#define KIRBYSKILLMAXXSIZE 50

class KirbySkill : public Skill
{
private:
	float biggersize;
	float decelerationrate;
	const char* imageaddress;

	Collider2D* collider;
public:
	KirbySkill(
		int masternum,
		int targetnum)
		: Skill(masternum, targetnum, SKILLTYPE::KIRBYSKILL, COLLIDERTYPE::CIRCLE, 5, 10, 5, 5., { totalData.udata[masternum].pos.x,totalData.udata[masternum].pos.y }, { 5,0 }),
		biggersize(1),
		decelerationrate(1),
		imageaddress(nullptr)
	{
		Circle2D* circle2D = new Circle2D(true, PMISSILE);
		circle2D->SetPosition(this->Getposition());
		SetCollider(circle2D);
	}

	Collider2D* GetCollider()	{ return collider; }
	float		Getbiggersize()	{ return biggersize; }
	float		Getdecelerationrate()	{ return decelerationrate; }

	void SetCollider(Collider2D* collider) override		{ this->collider = collider; }
	void Setbiggersize(float biggersize)	{ this->biggersize = biggersize; }
	void Setdecelerationrate(float decelerationrate)	{ this->decelerationrate = decelerationrate; }

	void KirbySkillBigger();
	void KirbySkillSlower();
};

void SetKirbySkillInDatasheet(Skill*& skill, int& skillnum)
{
	KirbySkill*	kirbyskill = dynamic_cast<KirbySkill*>(skill);
	Circle2D*	kirbycollider = dynamic_cast<Circle2D*>(kirbyskill->GetCollider());

	totalData.sdata[skillnum].isactivate = true;
	totalData.sdata[skillnum].skilltype = skill->Getskilltype();
	totalData.sdata[skillnum].size = skill->Getsize();
	totalData.sdata[skillnum].position = skill->Getposition();
	totalData.sdata[skillnum].colliderposition = kirbyskill->GetCollider()->GetPosition();
	totalData.sdata[skillnum].collidersize = kirbycollider->GetRadius();
	totalData.sdata[skillnum].collidertype = skill->Getcollidertype();
}

KirbySkill* kirbyskill = nullptr;
void UpdateKirbySkill(Skill* &skill)
{
	kirbyskill = dynamic_cast<KirbySkill*>(skill);

	POINT newpos;
	newpos.x = kirbyskill->Getposition().x + kirbyskill->Getdirection().x;
	newpos.y = kirbyskill->Getposition().y + kirbyskill->Getdirection().y;
	kirbyskill->Setposition(newpos);

	if (kirbyskill->Getsize() < KIRBYSKILLMAXXSIZE)
		kirbyskill->KirbySkillBigger();

	kirbyskill->GetCollider()->SetPosition(kirbyskill->Getposition());
	Circle2D* circle = dynamic_cast<Circle2D*>(kirbyskill->GetCollider());
	circle->SetRadius(kirbyskill->Getsize());
	
	kirbyskill->SetCollider(circle);
}

void KirbySkill::KirbySkillBigger()
{
	this->Setsize(this->Getsize() + this->Getbiggersize());
}

void KirbySkill::KirbySkillSlower()
{
	this->Setspeed(this->Getspeed());
}