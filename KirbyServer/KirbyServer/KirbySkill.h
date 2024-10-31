#pragma once

#include "Skill.h"

#define KIRBYSKILLMAXXSIZE 50

class KirbySkill : public Skill
{
private:
	float biggersize;
	float decelerationrate;
public:
	KirbySkill(
		int masternum,
		int targetnum,
		int radius )
		: Skill(masternum, targetnum, SKILLTYPE::KIRBYSKILL, ECOLLIDERSHAPE::CIRCLE,
			0.1, 10, 10, 5, 5, 3., { 0,0 }, { totalData.udata[masternum].pos.x, totalData.udata[masternum].pos.y }, { 5,0 }),
		biggersize(1),
		decelerationrate(0.01)
	{
		Circle2D* circle2D = new Circle2D(true, PMISSILE, radius);
		circle2D->SetPosition(this->Getposition());
		SetCollider(circle2D);

		totalData.udata[Getmasternum()].curState = (ECharacterState)PATTACK;
	}
	~KirbySkill()
	{
		/*delete imageaddress;
		delete collider;*/
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

static KirbySkill* kirbyskill = nullptr;

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

	kirbyskill->Sett2_destroy();
	double destroyTime = std::chrono::duration_cast<std::chrono::duration<double>>(kirbyskill->Gett2_destroy() - kirbyskill->Gett1_destroy()).count();

	kirbyskill->Sett2_attacktick();
	double hittime = std::chrono::duration_cast<std::chrono::duration<double>>(kirbyskill->Gett2_attacktick() - kirbyskill->Gett1_attacktick()).count();
	
	if (hittime > KIRBYTICK)
	{
		kirbyskill->Setcanhit(true);
		kirbyskill->Sett1_attacktick();
	}
	else
	{
		kirbyskill->Setcanhit(false);
	}

	if (totalData.udata[kirbyskill->Getmasternum()].curState == (ECharacterState)PATTACK
		&& destroyTime > KIRBY_SKILL_END_TIME)
	{
		totalData.udata[kirbyskill->Getmasternum()].curState = (ECharacterState)PIDLE;
	}

	if (destroyTime > TKIRBYSKILLDESTROY)
	{
		OBJECTIDARR[kirbyskill->GetID()] = false;
		delete skill;
		skill = nullptr;
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