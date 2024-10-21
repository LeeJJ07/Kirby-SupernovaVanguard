#pragma once

#include "Skill.h"

class MetaknightSkill : public Skill
{
private:
	const char* imageaddress;

	Collider2D* collider;

	std::chrono::high_resolution_clock::time_point t1_activate;
	std::chrono::high_resolution_clock::time_point t2_activate;
public:
	MetaknightSkill(
		int masternum,
		int targetnum)
		: Skill(masternum, targetnum, SKILLTYPE::METAKNIGHTSKILL, COLLIDERTYPE::RECTANGLE, 0, 10, 40, 2., { 0,0 } , { totalData.udata[masternum].pos.x, totalData.udata[masternum].pos.y }, { 5,0 }),
		imageaddress(nullptr)
	{
		Rectangle2D* rectangle2D = new Rectangle2D(true, PMISSILE);
		rectangle2D->SetPosition(this->Getposition());
		SetCollider(rectangle2D);
	}
	~MetaknightSkill()
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

bool SetMetaknightSkillInDatasheet(Skill*& skill, int& ID)
{
	MetaknightSkill* metaknightskill = dynamic_cast<MetaknightSkill*>(skill);
	Rectangle2D* metaknightcollider = dynamic_cast<Rectangle2D*>(metaknightskill->GetCollider());

	totalData.sdata[ID].isactivate = metaknightskill->Getisactivate();
	totalData.sdata[ID].id = metaknightskill->GetID();
	totalData.sdata[ID].skilltype = skill->Getskilltype();
	totalData.sdata[ID].size = skill->Getsize();
	totalData.sdata[ID].position = skill->Getposition();
	totalData.sdata[ID].colliderposition = metaknightskill->GetCollider()->GetPosition();
	totalData.sdata[ID].collidersize = metaknightcollider->GetSize();
	totalData.sdata[ID].collidertype = skill->Getcollidertype();

	return true;
}

MetaknightSkill* metaknightskill = nullptr;

void UpdateMetaknightSkill(Skill*& skill)
{
	metaknightskill = dynamic_cast<MetaknightSkill*>(skill);

	POINT newpos;
	newpos.x = metaknightskill->Getposition().x + metaknightskill->Getdirection().x * metaknightskill->Getspeed();
	newpos.y = metaknightskill->Getposition().y + metaknightskill->Getdirection().y * metaknightskill->Getspeed();
	metaknightskill->Setposition(newpos);

	metaknightskill->GetCollider()->SetPosition(metaknightskill->Getposition());
	Rectangle2D* rectangle = dynamic_cast<Rectangle2D*>(metaknightskill->GetCollider());
	rectangle->SetSize(metaknightskill->Getsize());

	metaknightskill->SetCollider(rectangle);

	metaknightskill->Settime_2();
	double skilldestroytime = std::chrono::duration_cast<std::chrono::duration<double>>(metaknightskill->Gettime_2() - metaknightskill->Gettime_1()).count();
	if (skilldestroytime > TMETAKNIGHTSKILLDESTROY)
	{
		metaknightskill->Setisactivate(false);
		OBJECTIDARR[metaknightskill->GetID()] = false;
	}
}