#pragma once

#include "Skill.h"

class MagicArrowSkill : public Skill
{
private:
	int pass;
	Collider2D* collider;

	std::chrono::high_resolution_clock::time_point t1_activate;
	std::chrono::high_resolution_clock::time_point t2_activate;
public:
	MagicArrowSkill(
		int masternum,
		int targetnum)
		: Skill(masternum, targetnum, SKILLTYPE::MAGICARROWSKILL, ECOLLIDERSHAPE::RECTANGLE, 0.6, 5, 3, 50, 15, 2., { 0,0 }, { totalData.udata[masternum].pos.x, totalData.udata[masternum].pos.y }, { 5,0 })
	{
		Rectangle2D* rectangle2D = new Rectangle2D(true, PMISSILE);
		rectangle2D->SetPosition(this->Getposition());
		SetCollider(rectangle2D);
	}
	~MagicArrowSkill()
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

MagicArrowSkill* magicarrowskill = nullptr;

bool SetMagicArrowSkillInDatasheet(Skill*& skill, int& ID)
{
	magicarrowskill = dynamic_cast<MagicArrowSkill*>(skill);
	Rectangle2D* magicarrowcollider = dynamic_cast<Rectangle2D*>(magicarrowskill->GetCollider());

	totalData.sdata[ID].isActivate = magicarrowskill->Getisactivate();
	totalData.sdata[ID].id = magicarrowskill->GetID();
	totalData.sdata[ID].skillType = skill->Getskilltype();
	totalData.sdata[ID].size = skill->Getsize();
	totalData.sdata[ID].size2 = skill->Getsize2();
	totalData.sdata[ID].position = skill->Getposition();
	totalData.sdata[ID].colliderPosition = magicarrowskill->GetCollider()->GetPosition();
	totalData.sdata[ID].colliderSize = magicarrowcollider->GetWidth();
	totalData.sdata[ID].colliderSize2 = magicarrowcollider->GetHeight();
	totalData.sdata[ID].colliderShape = skill->GetcolliderShape();
	totalData.sdata[ID].targetnum = skill->Gettargetnum();
	totalData.sdata[ID].angle = skill->Getangle();
	totalData.sdata[ID].dataType = SKILLTYPE;

	return true;
}

void UpdateMagicArrowSkill(Skill*& skill)
{
	magicarrowskill = dynamic_cast<MagicArrowSkill*>(skill);

	POINT newpos;
	newpos.x = magicarrowskill->Getposition().x + magicarrowskill->Getdirection().x * magicarrowskill->Getspeed();
	newpos.y = magicarrowskill->Getposition().y + magicarrowskill->Getdirection().y * magicarrowskill->Getspeed();
	magicarrowskill->Setposition(newpos);

	magicarrowskill->GetCollider()->SetPosition(magicarrowskill->Getposition());
	Rectangle2D* rectangle = dynamic_cast<Rectangle2D*>(magicarrowskill->GetCollider());
	rectangle->SetWidth(magicarrowskill->Getsize());
	rectangle->SetHeight(magicarrowskill->Getsize2());

	magicarrowskill->SetCollider(rectangle);

	magicarrowskill->Settime_2();
	double skilldestroytime = std::chrono::duration_cast<std::chrono::duration<double>>(magicarrowskill->Gettime_2() - magicarrowskill->Gettime_1()).count();

	magicarrowskill->Sett2_attacktick();
	double hittime = std::chrono::duration_cast<std::chrono::duration<double>>(magicarrowskill->Gett2_attacktick() - magicarrowskill->Gett1_attacktick()).count();

	if (hittime > MAGICARROWTICK)
	{
		magicarrowskill->Setcanhit(true);
		magicarrowskill->Sett1_attacktick();
	}
	else
	{
		magicarrowskill->Setcanhit(false);
	}

	if (skilldestroytime > TMAGICARROWSKILLDESTROY)
	{
		OBJECTIDARR[magicarrowskill->GetID()] = false;
		delete skill;
		skill = nullptr;
	}
}