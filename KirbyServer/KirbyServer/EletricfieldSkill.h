#pragma once

#include "Skill.h"

class ElectricfieldSkill : public Skill
{
private:
	const char* imageaddress;

	Collider2D* collider;

	std::chrono::high_resolution_clock::time_point t1_activate;
	std::chrono::high_resolution_clock::time_point t2_activate;
public:
	ElectricfieldSkill(
		int masternum,
		int targetnum)
		: Skill(masternum, targetnum, SKILLTYPE::ELECTRICFIELDSKILL, COLLIDERTYPE::CIRCLE, 0, 5, 80, 0, 1.2, { 0,0 }, { totalData.udata[masternum].pos.x, totalData.udata[masternum].pos.y }, { 5,0 }),
		imageaddress(nullptr)
	{
		Circle2D* circle2D = new Circle2D(true, PMISSILE);
		circle2D->SetPosition(this->Getposition());
		SetCollider(circle2D);
	}
	~ElectricfieldSkill()
	{
		delete imageaddress;
		delete collider;
	}

	Collider2D* GetCollider() { return collider; }
	std::chrono::high_resolution_clock::time_point	Gettime_1() { return t1_activate; }
	std::chrono::high_resolution_clock::time_point	Gettime_2() { return t2_activate; }


	void SetCollider(Collider2D* collider) override { this->collider = collider; }

	void Settime_1() { t1_activate = std::chrono::high_resolution_clock::now(); }
	void Settime_2() { t2_activate = std::chrono::high_resolution_clock::now(); }
};

ElectricfieldSkill* electricfieldskill = nullptr;

bool SetElectricfieldSkillInDatasheet(Skill*& skill, int& ID)
{
	electricfieldskill = dynamic_cast<ElectricfieldSkill*>(skill);
	Circle2D* electriccollider = dynamic_cast<Circle2D*>(electricfieldskill->GetCollider());

	totalData.sdata[ID].isactivate = electricfieldskill->Getisactivate();
	totalData.sdata[ID].id = electricfieldskill->GetID();
	totalData.sdata[ID].skilltype = skill->Getskilltype();
	totalData.sdata[ID].size = skill->Getsize();
	totalData.sdata[ID].position = skill->Getposition();
	totalData.sdata[ID].colliderposition = electricfieldskill->GetCollider()->GetPosition();
	totalData.sdata[ID].collidersize = electriccollider->GetRadius();
	totalData.sdata[ID].collidertype = skill->Getcollidertype();
	totalData.sdata[ID].targetnum = skill->Gettargetnum();

	return true;
}

void UpdateElectricfieldSkill(Skill*& skill)
{
	electricfieldskill = dynamic_cast<ElectricfieldSkill*>(skill);

	POINT newpos;
	newpos.x = electricfieldskill->Getposition().x + electricfieldskill->Getdirection().x * electricfieldskill->Getspeed();
	newpos.y = electricfieldskill->Getposition().y + electricfieldskill->Getdirection().y * electricfieldskill->Getspeed();
	electricfieldskill->Setposition(newpos);

	electricfieldskill->GetCollider()->SetPosition(electricfieldskill->Getposition());
	Circle2D* circle = dynamic_cast<Circle2D*>(electricfieldskill->GetCollider());
	circle->SetRadius(electricfieldskill->Getsize());

	electricfieldskill->SetCollider(circle);

	electricfieldskill->Settime_2();
	double skilldestroytime = std::chrono::duration_cast<std::chrono::duration<double>>(electricfieldskill->Gettime_2() - electricfieldskill->Gettime_1()).count();
	if (skilldestroytime > 0.1)
	{
		electricfieldskill->Setisactivate(false);
		OBJECTIDARR[electricfieldskill->GetID()] = false;
	}
}