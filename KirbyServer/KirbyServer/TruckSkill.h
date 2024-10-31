#pragma once

#include "Skill.h"
//#include "Monster.h"

class TruckSkill : public Skill
{
private:
	Collider2D* collider;

	std::chrono::high_resolution_clock::time_point t1_activate;
	std::chrono::high_resolution_clock::time_point t2_activate;
public:
	TruckSkill(
		int masternum,
		int targetnum)
		: Skill(masternum, targetnum, SKILLTYPE::TRUCKSKILL, ECOLLIDERSHAPE::RECTANGLE, 0.25, 100, 30, 150, 100, 10., { 0,0 }, { totalData.udata[masternum].pos.x, totalData.udata[masternum].pos.y }, { 5,0 })
	{
		Rectangle2D* rectangle2D = new Rectangle2D(true, PMISSILE);
		rectangle2D->SetPosition(this->Getposition());
		SetCollider(rectangle2D);
	}
	~TruckSkill()
	{   
		delete collider;
	}

	Collider2D* GetCollider() { return collider; }
	std::chrono::high_resolution_clock::time_point	Gettime_1() { return t1_activate; }
	std::chrono::high_resolution_clock::time_point	Gettime_2() { return t2_activate; }


	void SetCollider(Collider2D* collider) override { this->collider = collider; }
	void Settime_1() { t1_activate = std::chrono::high_resolution_clock::now(); }
	void Settime_2() { t2_activate = std::chrono::high_resolution_clock::now(); }
};

TruckSkill* truckskill = nullptr;

void UpdateTruckSkill(Skill*& skill)
{
	truckskill = dynamic_cast<TruckSkill*>(skill);

	POINT newpos;
	newpos.x = truckskill->Getposition().x + truckskill->Getdirection().x * truckskill->Getspeed();
	newpos.y = truckskill->Getposition().y + truckskill->Getdirection().y * truckskill->Getspeed();
	truckskill->Setposition(newpos);

	truckskill->GetCollider()->SetPosition(truckskill->Getposition());
	Rectangle2D* rectangle = dynamic_cast<Rectangle2D*>(truckskill->GetCollider());
	rectangle->SetWidth(truckskill->Getsize());
	rectangle->SetHeight(truckskill->Getsize2());

	truckskill->SetCollider(rectangle);

	truckskill->Settime_2();
	double skilldestroytime = std::chrono::duration_cast<std::chrono::duration<double>>(truckskill->Gettime_2() - truckskill->Gettime_1()).count();

	truckskill->Sett2_attacktick();
	double hittime = std::chrono::duration_cast<std::chrono::duration<double>>(truckskill->Gett2_attacktick() - truckskill->Gett1_attacktick()).count();

	if (hittime > TRUCKTICK)
	{
		truckskill->Setcanhit(true);
		truckskill->Sett1_attacktick();
	}
	else
	{
		truckskill->Setcanhit(false);
	}

	if (skilldestroytime > TTRUCKSKILLDESTROY)
	{
		OBJECTIDARR[truckskill->GetID()] = false;
		delete skill;
		skill = nullptr;
	}
}