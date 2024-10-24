#pragma once

#include "MonsterSkill.h"

class FiremanSkill : public MonsterSkill
{
private:
	Collider2D* collider;

	std::chrono::high_resolution_clock::time_point t1_activate;
	std::chrono::high_resolution_clock::time_point t2_activate;

public:
	FiremanSkill(
		int masternum,
		int targetnum)
		: MonsterSkill(masternum, targetnum, MONSTERSKILLTYPE::FIREMANSKILL, ECOLLIDERSHAPE::CIRCLE, 0, 20, 50, 0, 0.5, { 0,0 }, { totalData.mdata[masternum].pos.x, totalData.mdata[masternum].pos.y }, { 5,0 })
	{
		Circle2D* circle2D = new Circle2D(true, EMISSILE);
		circle2D->SetPosition(this->Getposition());
		SetCollider(circle2D);
	}
	~FiremanSkill()
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

FiremanSkill* firemanSkill = nullptr;

bool SetFiremanSkillInDatasheet(MonsterSkill*& monsterskill, int& ID)
{
	firemanSkill = dynamic_cast<FiremanSkill*>(monsterskill);
	Circle2D* firemancollider = dynamic_cast<Circle2D*>(firemanSkill->GetCollider());

	totalData.msdata[ID].isActivate = firemanSkill->Getisactivate();
	totalData.msdata[ID].id = firemanSkill->GetID();
	totalData.msdata[ID].colliderPosition = firemanSkill->GetCollider()->GetPosition();
	totalData.msdata[ID].colliderSize = firemancollider->GetRadius();
	totalData.msdata[ID].skillType = monsterskill->Getskilltype();
	totalData.msdata[ID].size = monsterskill->Getsize();
	totalData.msdata[ID].position = monsterskill->Getposition();
	totalData.msdata[ID].colliderShape = monsterskill->GetcolliderShape();
	totalData.msdata[ID].targetnum = monsterskill->Gettargetnum();
	totalData.msdata[ID].angle = monsterskill->Getangle();
	totalData.msdata[ID].dataType = SKILLTYPE;

	return true;
}

void UpdateFiremanSkill(MonsterSkill*& monsterskill)
{
	firemanSkill = dynamic_cast<FiremanSkill*>(monsterskill);

	POINT newpos;
	newpos.x = firemanSkill->Getposition().x + firemanSkill->Getdirection().x * firemanSkill->Getspeed();
	newpos.y = firemanSkill->Getposition().y + firemanSkill->Getdirection().y * firemanSkill->Getspeed();
	firemanSkill->Setposition(newpos);

	firemanSkill->GetCollider()->SetPosition(firemanSkill->Getposition());
	Circle2D* circle = dynamic_cast<Circle2D*>(firemanSkill->GetCollider());
	circle->SetRadius(firemanSkill->Getsize());

	firemanSkill->SetCollider(circle);

	firemanSkill->Settime_2();
	double skilldestroytime = std::chrono::duration_cast<std::chrono::duration<double>>(firemanSkill->Gettime_2() - firemanSkill->Gettime_1()).count();

	// 스킬 비활성화
	if (skilldestroytime > TFIREMANSKILLDESTROY)
	{
		firemanSkill->Setisactivate(false);
		OBJECTIDARR[firemanSkill->GetID()] = false;
	}
}