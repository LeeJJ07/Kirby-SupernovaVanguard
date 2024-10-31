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
		: Skill(masternum, targetnum, SKILLTYPE::TORNADOSKILL, ECOLLIDERSHAPE::RECTANGLE, 0.2, 10, 30, 50, 50, 6, { 0,0 }, { totalData.udata[masternum].pos.x, totalData.udata[masternum].pos.y }, { 5,0 })
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

void UpdateTornadoSkill(Skill*& skill)
{
	tornadoskill = dynamic_cast<TornadoSkill*>(skill);

	POINT myposition = skill->Getposition();

	int monsterIndex = FindCloseMonster(myposition);
	tornadoskill->Settargetnum(monsterIndex);

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

	tornadoskill->Sett2_attacktick();
	double hittime = std::chrono::duration_cast<std::chrono::duration<double>>(tornadoskill->Gett2_attacktick() - tornadoskill->Gett1_attacktick()).count();

	if (hittime > TORNADOTICK)
	{
		tornadoskill->Setcanhit(true);
		tornadoskill->Sett1_attacktick();
	}
	else
	{
		tornadoskill->Setcanhit(false);
	}

	if (skilldestroytime > TTORNADOSKILLDESTROY)
	{
		OBJECTIDARR[tornadoskill->GetID()] = false;
		delete skill;
		skill = nullptr;
	}
}