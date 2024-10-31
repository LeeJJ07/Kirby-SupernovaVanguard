#pragma once

#include "MonsterSkill.h"

#define FIREBALLNUM 24

class FireballSkill : public MonsterSkill
{
private:
	Collider2D* collider;

	std::chrono::high_resolution_clock::time_point t1_activate;
	std::chrono::high_resolution_clock::time_point t2_activate;

public:
	FireballSkill(
		int masternum,
		int targetnum,
		int radius)
		: MonsterSkill(masternum, targetnum, MONSTERSKILLTYPE::FIREBALLSKILL, ECOLLIDERSHAPE::CIRCLE, 1, 5, 20, 20, 4., { 0,0 }, { totalData.mdata[masternum].pos.x, totalData.mdata[masternum].pos.y }, { 5,0 })
	{
		Circle2D* circle2D = new Circle2D(true, EMISSILE, radius);
		circle2D->SetPosition(this->Getposition());
		SetCollider(circle2D);
	}
	~FireballSkill()
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

FireballSkill* fireballSkill = nullptr;

bool SetFireballSkillInDatasheet(MonsterSkill*& monsterskill, int& ID)
{
	fireballSkill = dynamic_cast<FireballSkill*>(monsterskill);
	Circle2D* fireballcollider = dynamic_cast<Circle2D*>(fireballSkill->GetCollider());

	totalData.msdata[ID].isActivate = fireballSkill->Getisactivate();
	totalData.msdata[ID].id = fireballSkill->GetID();
	totalData.msdata[ID].colliderPosition = fireballSkill->GetCollider()->GetPosition();
	totalData.msdata[ID].colliderSize = fireballcollider->GetRadius();
	totalData.msdata[ID].skillType = monsterskill->Getskilltype();
	totalData.msdata[ID].size = monsterskill->Getsize();
	totalData.msdata[ID].size2 = monsterskill->Getsize2();
	totalData.msdata[ID].position = monsterskill->Getposition();
	totalData.msdata[ID].colliderShape = monsterskill->GetcolliderShape();
	totalData.msdata[ID].targetnum = monsterskill->Gettargetnum(); 
	totalData.msdata[ID].angle = monsterskill->Getangle();
	totalData.msdata[ID].dataType = SKILLTYPE;

	return true;
}

void UpdateFireballSkill(MonsterSkill*& monsterskill)
{
	fireballSkill = dynamic_cast<FireballSkill*>(monsterskill);

	POINT newpos;
	newpos.x = fireballSkill->Getposition().x + fireballSkill->Getdirection().x * fireballSkill->Getspeed();
	newpos.y = fireballSkill->Getposition().y + fireballSkill->Getdirection().y * fireballSkill->Getspeed();
	fireballSkill->Setposition(newpos);

	fireballSkill->GetCollider()->SetPosition(fireballSkill->Getposition());
	Circle2D* circle = dynamic_cast<Circle2D*>(fireballSkill->GetCollider());
	circle->SetRadius(fireballSkill->Getsize());

	fireballSkill->SetCollider(circle);

	fireballSkill->Settime_2();
	double skilldestroytime = std::chrono::duration_cast<std::chrono::duration<double>>(fireballSkill->Gettime_2() - fireballSkill->Gettime_1()).count();

	// 스킬 비활성화
	if (skilldestroytime > TFIREBALLSKILLDESTROY)
	{
		fireballSkill->Setisactivate(false);
		OBJECTIDARR[fireballSkill->GetID()] = false;
	}
}