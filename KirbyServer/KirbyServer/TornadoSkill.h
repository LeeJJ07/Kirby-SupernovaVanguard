#pragma once

#include "Skill.h"

class TornadoSkill : public Skill
{
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

	void	AssignSkill(int&, PLAYERDATA&, MONSTERDATA&);
};

void TornadoSkill::AssignSkill(int& playerIndex, PLAYERDATA& playerData, MONSTERDATA& monsterData)
{
	PAIR lookingdir;
	Setdirection({ (long)playerData.lookingDir.first, (long)playerData.lookingDir.second });
	Setisactivate(true);
	Setoffset({ (long)playerData.lookingDir.first * (long)Getsize() / OFFSETADJUST / 2, (long)playerData.lookingDir.second * (long)Getsize() / OFFSETADJUST / 2 });
	Setposition({ playerData.pos.x + Getoffset().x, playerData.pos.y + Getoffset().y });
	Setmasternum(playerIndex);
}

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

	tornadoskill->Sett2_destroy();
	double skilldestroytime = std::chrono::duration_cast<std::chrono::duration<double>>(tornadoskill->Gett2_destroy() - tornadoskill->Gett1_destroy()).count();

	tornadoskill->Sett2_attacktick();
	double hittime = std::chrono::duration_cast<std::chrono::duration<double>>(tornadoskill->Gett2_attacktick() - tornadoskill->Gett1_attacktick()).count();

	tornadoskill->Sett1_attacktick();

	if (skilldestroytime > TTORNADOSKILLDESTROY)
	{
		OBJECTIDARR[tornadoskill->GetID()] = false;
		delete skill;
		skill = nullptr;
	}
}