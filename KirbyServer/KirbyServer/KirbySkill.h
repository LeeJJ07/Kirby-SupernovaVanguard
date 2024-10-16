#pragma once

#include "Skill.h"

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
		: Skill(masternum, targetnum, SKILLTYPE::KIRBY, COLLIDERTYPE::CIRCLE, 5, 10, 5, 5., { totalData.udata[masternum].pos.x,totalData.udata[masternum].pos.y }, { 1,0 }),
		biggersize(1),
		decelerationrate(1),
		imageaddress(nullptr)
	{
		Circle2D* circle2D = new Circle2D(true, PMISSILE);
		circle2D->SetPosition(this->Getposition());
		SetCollider(circle2D);
	}

	Collider2D* GetCollider() { return collider; }

	void SetCollider(Collider2D* collider) override { this->collider = collider; }
};

void SetBasisKirbySkillInDatasheet(Skill* skill)
{
	KirbySkill* kirbyskill = dynamic_cast<KirbySkill*>(skill);
	totalData.sdata[skill->GetID()];

	totalData.sdata[index].isactivate = true;
	totalData.sdata[index].skilltype = kirbyskill->Getskilltype();
	totalData.sdata[index].speed = kirbyskill->Getspeed();
	totalData.sdata[index].damage = kirbyskill->Getdamage();
	totalData.sdata[index].size = kirbyskill->Getsize();
	totalData.sdata[index].coolTime = kirbyskill->Getcooltime();
	totalData.sdata[index].position = kirbyskill->Getposition();
	
	vSkill.push_back(kirbyskill);
}

void UpdateKirbySkill(Skill* &skill)
{
	KirbySkill* kirbyskill = dynamic_cast<KirbySkill*>(skill);
	POINT newpos;
	newpos.x = skill->Getposition().x + skill->Getdirection().x;
	newpos.y = skill->Getposition().y + skill->Getdirection().y;
	kirbyskill->Setposition(newpos);
}