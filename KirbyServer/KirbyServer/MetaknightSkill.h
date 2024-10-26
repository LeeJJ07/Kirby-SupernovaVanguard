#pragma once

#include "Skill.h"

class MetaknightSkill : public Skill
{
private:
	Collider2D* collider;

	std::chrono::high_resolution_clock::time_point t1_activate;
	std::chrono::high_resolution_clock::time_point t2_activate;
public:
	MetaknightSkill(
		int masternum,
		int targetnum)
		: Skill(masternum, targetnum, SKILLTYPE::METAKNIGHTSKILL, ECOLLIDERSHAPE::RECTANGLE, 0, 10, 100, 80, 2., { 0,0 } , { totalData.udata[masternum].pos.x, totalData.udata[masternum].pos.y }, { 5,0 })
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

MetaknightSkill* metaknightskill = nullptr;

bool SetMetaknightSkillInDatasheet(Skill*& skill, int& ID)
{
	metaknightskill = dynamic_cast<MetaknightSkill*>(skill);
	Rectangle2D* metaknightcollider = dynamic_cast<Rectangle2D*>(metaknightskill->GetCollider());

	totalData.sdata[ID].isActivate = metaknightskill->Getisactivate();
	totalData.sdata[ID].id = metaknightskill->GetID();
	totalData.sdata[ID].skillType = skill->Getskilltype();
	totalData.sdata[ID].size = skill->Getsize();
	totalData.sdata[ID].size2 = skill->Getsize2();
	totalData.sdata[ID].position = skill->Getposition();
	totalData.sdata[ID].colliderPosition = metaknightskill->GetCollider()->GetPosition();
	totalData.sdata[ID].colliderSize = metaknightcollider->GetWidth();
	totalData.sdata[ID].colliderSize2 = metaknightcollider->GetHeight();
	totalData.sdata[ID].colliderShape = skill->GetcolliderShape();
	totalData.sdata[ID].targetnum = skill->Gettargetnum();
	totalData.sdata[ID].angle = skill->Getangle();
	totalData.sdata[ID].dataType = SKILLTYPE;

	return true;
}

void UpdateMetaknightSkill(Skill*& skill)
{
	metaknightskill = dynamic_cast<MetaknightSkill*>(skill);

	POINT newpos;
	newpos.x = totalData.udata[metaknightskill->Getmasternum()].pos.x + metaknightskill->Getoffset().x;
	newpos.y = totalData.udata[metaknightskill->Getmasternum()].pos.y + metaknightskill->Getoffset().y;
	metaknightskill->Setposition(newpos);

	metaknightskill->GetCollider()->SetPosition(metaknightskill->Getposition());
	Rectangle2D* rectangle = dynamic_cast<Rectangle2D*>(metaknightskill->GetCollider());
	rectangle->SetWidth(metaknightskill->Getsize());
	rectangle->SetHeight(metaknightskill->Getsize2());

	metaknightskill->SetCollider(rectangle);

	metaknightskill->Settime_2();
	double skilldestroytime = std::chrono::duration_cast<std::chrono::duration<double>>(metaknightskill->Gettime_2() - metaknightskill->Gettime_1()).count();
	if (skilldestroytime > 0.5)
	{
		OBJECTIDARR[metaknightskill->GetID()] = false;
		delete skill;
		skill = nullptr;
	}
}