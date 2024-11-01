#pragma once

#include "Skill.h"

class KunaiSkill : public Skill
{
public:
	KunaiSkill(
		int masternum,
		int targetnum)
		: Skill(masternum, targetnum, SKILLTYPE::KUNAISKILL, ECOLLIDERSHAPE::RECTANGLE, 0.8, 4, 1, 30, 5, 0.25, { 0,0 }, { totalData.udata[masternum].pos.x, totalData.udata[masternum].pos.y }, { 5,0 })
	{
		Rectangle2D* rectangle2D = new Rectangle2D(true, PMISSILE);
		rectangle2D->SetPosition(this->Getposition());
		SetCollider(rectangle2D);
	}
	~KunaiSkill()
	{
		delete collider;
	}

	void	AssignSkill(int&, PLAYERDATA&, MONSTERDATA&);
};

void KunaiSkill::AssignSkill(int& playerIndex, PLAYERDATA& playerData, MONSTERDATA& monsterData)
{
	PAIR lookingdir;
	if (!(vClient[playerIndex]->GetisLockOn()))
	{
		lookingdir = { (long)playerData.lookingDir.first, (long)playerData.lookingDir.second };
		Setdirection({ (long)lookingdir.first, (long)lookingdir.second });
	}
	else
	{
		lookingdir = { (Getposition().x - monsterData.pos.x), (Getposition().y - monsterData.pos.y) };

		if (lookingdir.first == 0)
			lookingdir.first = 0.1f;
		if (lookingdir.second == 0)
			lookingdir.second = 0.1f;

		double temp = sqrt(pow(lookingdir.first, 2) + pow(lookingdir.second, 2));
		lookingdir.first /= -temp / OFFSETADJUST; lookingdir.second /= -temp / OFFSETADJUST;

		Setdirection({ (long)lookingdir.first,(long)lookingdir.second });
	}
	Setangle(LookingDirToDegree(lookingdir));
	Setisactivate(true);
	Setoffset({ (long)playerData.lookingDir.first * (long)Getsize() / OFFSETADJUST / 2, (long)playerData.lookingDir.second * (long)Getsize() / OFFSETADJUST / 2 });
	Setposition({ playerData.pos.x + Getoffset().x, playerData.pos.y + Getoffset().y });
	Setmasternum(playerIndex);
}

KunaiSkill* kunaiskill = nullptr;

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

	kunaiskill->Sett2_destroy();
	double skilldestroytime = std::chrono::duration_cast<std::chrono::duration<double>>(kunaiskill->Gett2_destroy() - kunaiskill->Gett1_destroy()).count();

	kunaiskill->Sett2_attacktick();
	double hittime = std::chrono::duration_cast<std::chrono::duration<double>>(kunaiskill->Gett2_attacktick() - kunaiskill->Gett1_attacktick()).count();

	kunaiskill->Sett1_attacktick();

	if (skilldestroytime > TKUNAISKILLDESTROY)
	{
		OBJECTIDARR[kunaiskill->GetID()] = false;
		delete skill;
		skill = nullptr;
	}
}