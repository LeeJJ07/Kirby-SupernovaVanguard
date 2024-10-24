#pragma once

#include "MonsterSkill.h"

class SpearSkill : public MonsterSkill
{
private:
	Collider2D* collider;

	std::chrono::high_resolution_clock::time_point t1_activate;
	std::chrono::high_resolution_clock::time_point t2_activate;

public:
	SpearSkill(
		int masternum,
		int targetnum)
		: MonsterSkill(masternum, targetnum, MONSTERSKILLTYPE::SPEARSKILL, ECOLLIDERSHAPE::RECTANGLE, 0.2, 5, 20, 5, 3., { 0,0 }, { totalData.mdata[masternum].pos.x, totalData.mdata[masternum].pos.y }, { 5,0 })
	{
		Rectangle2D* rectangle2D = new Rectangle2D(true, EMISSILE);
		rectangle2D->SetPosition(this->Getposition());
		SetCollider(rectangle2D);
	}
	~SpearSkill()
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

SpearSkill* spearSkill = nullptr;

bool SetSpearSkillInDatasheet(MonsterSkill*& monsterskill, int& ID)
{
	spearSkill = dynamic_cast<SpearSkill*>(monsterskill);
	Rectangle2D* spearcollider = dynamic_cast<Rectangle2D*>(spearSkill->GetCollider());

	totalData.msdata[ID].isActivate = spearSkill->Getisactivate();
	totalData.msdata[ID].id = spearSkill->GetID();
	totalData.msdata[ID].colliderPosition = spearSkill->GetCollider()->GetPosition();
	totalData.msdata[ID].colliderSize = spearcollider->GetWidth();
	totalData.msdata[ID].colliderSize2 = spearcollider->GetHeight();
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

void UpdateSpearSkill(MonsterSkill*& monsterskill)
{
	spearSkill = dynamic_cast<SpearSkill*>(monsterskill);

	POINT newpos;
	newpos.x = spearSkill->Getposition().x + spearSkill->Getdirection().x * spearSkill->Getspeed();
	newpos.y = spearSkill->Getposition().y + spearSkill->Getdirection().y * spearSkill->Getspeed();
	spearSkill->Setposition(newpos);

	spearSkill->GetCollider()->SetPosition(spearSkill->Getposition());
	Rectangle2D* rectangle = dynamic_cast<Rectangle2D*>(spearSkill->GetCollider());
	rectangle->SetWidth(spearSkill->Getsize());
	rectangle->SetHeight(spearSkill->Getsize2());

	spearSkill->SetCollider(rectangle);

	spearSkill->Settime_2();
	double skilldestroytime = std::chrono::duration_cast<std::chrono::duration<double>>(spearSkill->Gettime_2() - spearSkill->Gettime_1()).count();

	// 스킬 비활성화
	if (skilldestroytime > TSPEARSKILLDESTROY)
	{
		spearSkill->Setisactivate(false);
		OBJECTIDARR[spearSkill->GetID()] = false;
	}
}