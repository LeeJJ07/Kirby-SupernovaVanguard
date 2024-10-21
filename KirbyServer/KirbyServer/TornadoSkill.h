#pragma once

#include "Skill.h"

#define WALL 5

class TornadoSkill : public Skill
{
private:
	const char* imageaddress;
	short	turnAround;
	short	wall;

	Collider2D* collider;

	std::chrono::high_resolution_clock::time_point t1_activate;
	std::chrono::high_resolution_clock::time_point t2_activate;
public:
	TornadoSkill(
		int masternum,
		int targetnum)
		: Skill(masternum, targetnum, SKILLTYPE::TORNADOSKILL, COLLIDERTYPE::RECTANGLE, 2, 10, 20, 40, 8., { 0,0 }, { totalData.udata[masternum].pos.x, totalData.udata[masternum].pos.y }, { 5,0 }),
		imageaddress(nullptr), turnAround(1), wall(0)
	{
		Rectangle2D* rectangle2D = new Rectangle2D(true, PMISSILE);
		rectangle2D->SetPosition(this->Getposition());
		SetCollider(rectangle2D);
	}
	~TornadoSkill()
	{
		delete imageaddress;
		delete collider;
	}

	short	GetTurnAround() { return turnAround; }
	short	GetWall() { return wall; }
	Collider2D* GetCollider() { return collider; }
	std::chrono::high_resolution_clock::time_point	Gettime_1() { return t1_activate; }
	std::chrono::high_resolution_clock::time_point	Gettime_2() { return t2_activate; }

	void	SetTurnAround(short turnAround) { this->turnAround = turnAround; }
	void	SetWall(short wall) { this->wall = wall; }
	void	SetCollider(Collider2D* collider) override { this->collider = collider; }
	void	Settime_1() { t1_activate = std::chrono::high_resolution_clock::now(); }
	void	Settime_2() { t2_activate = std::chrono::high_resolution_clock::now(); }
};

TornadoSkill* tornadoskill = nullptr;

bool SetTornadoSkillInDatasheet(Skill*& skill, int& ID)
{
	tornadoskill = dynamic_cast<TornadoSkill*>(skill);
	Rectangle2D* tornadocollider = dynamic_cast<Rectangle2D*>(tornadoskill->GetCollider());

	totalData.sdata[ID].isactivate	= tornadoskill->Getisactivate();
	totalData.sdata[ID].id	= tornadoskill->GetID();
	totalData.sdata[ID].skilltype	= skill->Getskilltype();
	totalData.sdata[ID].size	= skill->Getsize();
	totalData.sdata[ID].size2	= skill->Getsize2();
	totalData.sdata[ID].position	= skill->Getposition();
	totalData.sdata[ID].colliderposition	= tornadoskill->GetCollider()->GetPosition();
	totalData.sdata[ID].collidersize	= tornadocollider->GetWidth();
	totalData.sdata[ID].collidersize2	= tornadocollider->GetHeight();
	totalData.sdata[ID].collidertype	= skill->Getcollidertype();

	return true;
}

void UpdateTornadoSkill(Skill*& skill)
{
	int playerindex = skill->Getmasternum();
	int monsterIndex = FindCloseMonster(playerindex);
	tornadoskill = dynamic_cast<TornadoSkill*>(skill);

	if (monsterIndex != -1)
	{
		POINT direction = GetNormalizationRange(totalData.udata[playerindex].pos, totalData.mdata[monsterIndex].pos);

		if (abs(tornadoskill->GetWall()) == WALL)
		{
			tornadoskill->SetTurnAround(-tornadoskill->GetTurnAround());
		}

		tornadoskill->SetWall(tornadoskill->GetWall() + tornadoskill->GetTurnAround());

		if (direction.x == 0 && direction.y == 0)
		{
			POINT newpos;
			newpos.x = tornadoskill->Getposition().x + tornadoskill->GetTurnAround() * tornadoskill->Getspeed();
			newpos.y = tornadoskill->Getposition().y;
			tornadoskill->Setposition(newpos);
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