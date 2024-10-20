#pragma once

#include "Skill.h"

class KunaiSkill : public Skill
{
private:
	const char* imageaddress;

	Collider2D* collider;

	std::chrono::high_resolution_clock::time_point t1_activate;
	std::chrono::high_resolution_clock::time_point t2_activate;
public:
	KunaiSkill(
		int masternum,
		int targetnum)
		: Skill(masternum, targetnum, SKILLTYPE::KUNAISKILL, COLLIDERTYPE::RECTANGLE, 4, 2, 5, 8, 0.4, { 0,0 }, { totalData.udata[masternum].pos.x, totalData.udata[masternum].pos.y }, { 5,0 }),
		imageaddress(nullptr)
	{
		Rectangle2D* rectangle2D = new Rectangle2D(true, PMISSILE);
		rectangle2D->SetPosition(this->Getposition());
		SetCollider(rectangle2D);
	}
	~KunaiSkill()
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

KunaiSkill* kunaiskill = nullptr;

bool SetKunaiSkillInDatasheet(Skill*& skill, int& ID)
{
	kunaiskill = dynamic_cast<KunaiSkill*>(skill);
	Rectangle2D* kunaicollider = dynamic_cast<Rectangle2D*>(kunaiskill->GetCollider());

	totalData.sdata[ID].isactivate = kunaiskill->Getisactivate();
	totalData.sdata[ID].id = kunaiskill->GetID();
	totalData.sdata[ID].skilltype = skill->Getskilltype();
	totalData.sdata[ID].size = skill->Getsize();
	totalData.sdata[ID].position = skill->Getposition();
	totalData.sdata[ID].colliderposition = kunaiskill->GetCollider()->GetPosition();
	totalData.sdata[ID].collidersize = kunaicollider->GetWidth();
	totalData.sdata[ID].collidersize2 = kunaicollider->GetHeight();
	totalData.sdata[ID].collidertype = skill->Getcollidertype();

	return true;
}

void UpdateKunaiSkill(Skill*& skill)
{
	kunaiskill = dynamic_cast<KunaiSkill*>(skill);

	POINT newpos;
	newpos.x = kunaiskill->Getposition().x + kunaiskill->Getdirection().x * kunaiskill->Getspeed();
	newpos.y = kunaiskill->Getposition().y + kunaiskill->Getdirection().y * kunaiskill->Getspeed();
	kunaiskill->Setposition(newpos);

	kunaiskill->GetCollider()->SetPosition(kunaiskill->Getposition());
	Rectangle2D* rectangle = dynamic_cast<Rectangle2D*>(kunaiskill->GetCollider());
	rectangle->SetWidth(kunaiskill->Getsize());
	rectangle->SetHeight(kunaiskill->Getsize2());

	kunaiskill->SetCollider(rectangle);

	kunaiskill->Settime_2();
	double skilldestroytime = std::chrono::duration_cast<std::chrono::duration<double>>(kunaiskill->Gettime_2() - kunaiskill->Gettime_1()).count();
	if (skilldestroytime > TKUNAISKILLDESTROY)
	{
		kunaiskill->Setisactivate(false);
		OBJECTIDARR[kunaiskill->GetID()] = false;
	}
}