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
		: Skill(masternum, targetnum, SKILLTYPE::TRUCKSKILL, ECOLLIDERSHAPE::RECTANGLE, 0.2, 50, 150, 100, 2., { 0,0 }, { totalData.udata[masternum].pos.x, totalData.udata[masternum].pos.y }, { 5,0 })
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

bool SetTruckSkillInDatasheet(Skill*& skill, int& ID)
{
	truckskill = dynamic_cast<TruckSkill*>(skill);
	Rectangle2D* tornadocollider = dynamic_cast<Rectangle2D*>(truckskill->GetCollider());

	totalData.sdata[ID].isActivate = truckskill->Getisactivate();
	totalData.sdata[ID].id = truckskill->GetID();
	totalData.sdata[ID].skillType = skill->Getskilltype();
	totalData.sdata[ID].size = skill->Getsize();
	totalData.sdata[ID].size2 = skill->Getsize2();
	totalData.sdata[ID].position = skill->Getposition();
	totalData.sdata[ID].colliderPosition = truckskill->GetCollider()->GetPosition();
	totalData.sdata[ID].colliderSize = tornadocollider->GetWidth();
	totalData.sdata[ID].colliderSize2 = tornadocollider->GetHeight();
	totalData.sdata[ID].colliderShape = skill->GetcolliderShape();
	totalData.sdata[ID].targetnum = skill->Gettargetnum();
	totalData.sdata[ID].angle = skill->Getangle();
	totalData.sdata[ID].dataType = SKILLTYPE;

	return true;
}

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
	if (skilldestroytime > TTRUCKSKILLDESTROY)
	{
		truckskill->Setisactivate(false);
		OBJECTIDARR[truckskill->GetID()] = false;
	}
}