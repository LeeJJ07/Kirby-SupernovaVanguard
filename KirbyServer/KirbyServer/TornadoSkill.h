#pragma once

#include "Skill.h"

class TornadoSkill : public Skill
{
private:
	Collider2D* collider;

	std::chrono::high_resolution_clock::time_point t1_activate;
	std::chrono::high_resolution_clock::time_point t2_activate;
public:
	TornadoSkill(
		int masternum,
		int targetnum)
		: Skill(masternum, targetnum, SKILLTYPE::TORNADOSKILL, COLLIDERTYPE::RECTANGLE, 0.2, 10, 30, 60, 6, { 0,0 }, { totalData.udata[masternum].pos.x, totalData.udata[masternum].pos.y }, { 5,0 })
	{
		Rectangle2D* rectangle2D = new Rectangle2D(true, PMISSILE);
		rectangle2D->SetPosition(this->Getposition());
		SetCollider(rectangle2D);
	}
	~TornadoSkill()
	{
		delete collider;
	}

	Collider2D* GetCollider() { return collider; }
	std::chrono::high_resolution_clock::time_point	Gettime_1() { return t1_activate; }
	std::chrono::high_resolution_clock::time_point	Gettime_2() { return t2_activate; }

	void	SetCollider(Collider2D* collider) override { this->collider = collider; }
	void	Settime_1() { t1_activate = std::chrono::high_resolution_clock::now(); }
	void	Settime_2() { t2_activate = std::chrono::high_resolution_clock::now(); }
};

TornadoSkill* tornadoskill = nullptr;

bool SetTornadoSkillInDatasheet(Skill*& skill, int& ID)
{
	tornadoskill = dynamic_cast<TornadoSkill*>(skill);
	Rectangle2D* tornadocollider = dynamic_cast<Rectangle2D*>(tornadoskill->GetCollider());

	totalData.sdata[ID].isActivate	= tornadoskill->Getisactivate();
	totalData.sdata[ID].id	= tornadoskill->GetID();
	totalData.sdata[ID].skillType	= skill->Getskilltype();
	totalData.sdata[ID].size	= skill->Getsize();
	totalData.sdata[ID].size2	= skill->Getsize2();
	totalData.sdata[ID].position	= skill->Getposition();
	totalData.sdata[ID].colliderPosition	= tornadoskill->GetCollider()->GetPosition();
	totalData.sdata[ID].colliderSize	= tornadocollider->GetWidth();
	totalData.sdata[ID].colliderSize2	= tornadocollider->GetHeight();
	totalData.sdata[ID].colliderType	= skill->Getcollidertype();
	totalData.sdata[ID].targetnum = skill->Gettargetnum();

	return true;
}

void UpdateTornadoSkill(Skill*& skill)
{
	tornadoskill = dynamic_cast<TornadoSkill*>(skill);

	POINT myposition = skill->Getposition();

	if (skill->Gettargetnum() == 0)
	{
		int monsterIndex = FindCloseMonster(myposition);
		tornadoskill->Settargetnum(monsterIndex);
	}

	if (tornadoskill->Gettargetnum() != -1)
	{
		POINT direction = GetNormalizationRange(myposition, totalData.mdata[tornadoskill->Gettargetnum()].pos);

		if (direction.x == 0 && direction.y == 0)
		{
			tornadoskill->Setposition(totalData.mdata[tornadoskill->Gettargetnum()].pos);
		}
		else
		{
			POINT newpos;
			newpos.x = tornadoskill->Getposition().x + direction.x * tornadoskill->Getspeed();
			newpos.y = tornadoskill->Getposition().y + direction.y * tornadoskill->Getspeed();
			tornadoskill->Setposition(newpos);
		}
	}

	tornadoskill->GetCollider()->SetPosition(tornadoskill->Getposition());
	Rectangle2D* rectangle = dynamic_cast<Rectangle2D*>(tornadoskill->GetCollider());
	rectangle->SetWidth(tornadoskill->Getsize());
	rectangle->SetHeight(tornadoskill->Getsize2());

	tornadoskill->SetCollider(rectangle);

	tornadoskill->Settime_2();
	double skilldestroytime = std::chrono::duration_cast<std::chrono::duration<double>>(tornadoskill->Gettime_2() - tornadoskill->Gettime_1()).count();
	if (skilldestroytime > TTORNADOSKILLDESTROY)
	{
		tornadoskill->Setisactivate(false);
		OBJECTIDARR[tornadoskill->GetID()] = false;
	}
}