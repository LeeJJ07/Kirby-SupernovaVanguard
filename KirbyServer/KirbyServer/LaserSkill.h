#pragma once

#include "MonsterSkill.h"

#define LASERNUM 6

class LaserSkill : public MonsterSkill
{
private:
	Collider2D* collider;

	std::chrono::high_resolution_clock::time_point t1_activate;
	std::chrono::high_resolution_clock::time_point t2_activate;

public:
	LaserSkill(
		int masternum,
		int targetnum)
		: MonsterSkill(masternum, targetnum, MONSTERSKILLTYPE::LASERSKILL, ECOLLIDERSHAPE::RECTANGLE, 2.5, 1, 100, 500, 4., { 0,0 }, { totalData.mdata[masternum].pos.x, totalData.mdata[masternum].pos.y }, { 5,0 })
	{
		Rectangle2D* rectangle2D = new Rectangle2D(true, EMISSILE);
		rectangle2D->SetPosition(this->Getposition());
		SetCollider(rectangle2D);
	}
	~LaserSkill()
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

LaserSkill* laserSkill = nullptr;

bool SetLaserSkillInDatasheet(MonsterSkill*& monsterskill, int& ID)
{
	laserSkill = dynamic_cast<LaserSkill*>(monsterskill);
	Rectangle2D* lasercollider = dynamic_cast<Rectangle2D*>(laserSkill->GetCollider());

	totalData.msdata[ID].isActivate = laserSkill->Getisactivate();
	totalData.msdata[ID].id = laserSkill->GetID();
	totalData.msdata[ID].colliderPosition = laserSkill->GetCollider()->GetPosition();
	totalData.msdata[ID].colliderSize = lasercollider->GetWidth();
	totalData.msdata[ID].colliderSize2 = lasercollider->GetHeight();
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

void UpdateLaserSkill(MonsterSkill*& monsterskill)
{
	laserSkill = dynamic_cast<LaserSkill*>(monsterskill);

	POINT newpos;
	newpos.x = laserSkill->Getposition().x + laserSkill->Getdirection().x * laserSkill->Getspeed();
	newpos.y = laserSkill->Getposition().y + laserSkill->Getdirection().y * laserSkill->Getspeed();
	laserSkill->Setposition(newpos);

	laserSkill->GetCollider()->SetPosition(laserSkill->Getposition());
	Rectangle2D* rectangle = dynamic_cast<Rectangle2D*>(laserSkill->GetCollider());
	rectangle->SetWidth(laserSkill->Getsize());
	rectangle->SetHeight(laserSkill->Getsize2());

	laserSkill->SetCollider(rectangle);

	laserSkill->Settime_2();
	double skilldestroytime = std::chrono::duration_cast<std::chrono::duration<double>>(laserSkill->Gettime_2() - laserSkill->Gettime_1()).count();

	// 스킬 비활성화
	if (skilldestroytime > TLASERSKILLDESTROY)
	{
		laserSkill->Setisactivate(false);
		OBJECTIDARR[laserSkill->GetID()] = false;
	}
}