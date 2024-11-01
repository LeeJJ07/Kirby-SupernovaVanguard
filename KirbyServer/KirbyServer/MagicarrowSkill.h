#pragma once

#include "Skill.h"

class MagicArrowSkill : public Skill
{
private:
	int pass;
public:
	MagicArrowSkill(
		int masternum,
		int targetnum)
		: Skill(masternum, targetnum, SKILLTYPE::MAGICARROWSKILL, ECOLLIDERSHAPE::RECTANGLE, 0.6, 20, 3, 50, 15, 2., { 0,0 }, { totalData.udata[masternum].pos.x, totalData.udata[masternum].pos.y }, { 5,0 })
	{
		Rectangle2D* rectangle2D = new Rectangle2D(true, PMISSILE);
		rectangle2D->SetPosition(this->Getposition());
		SetCollider(rectangle2D);
	}
	~MagicArrowSkill()
	{
		delete collider;
	}

	void	AssignSkill(int&, PLAYERDATA&, MONSTERDATA&);
};

void MagicArrowSkill::AssignSkill(int& playerIndex, PLAYERDATA& playerData, MONSTERDATA& monsterData)
{
	PAIR lookingdir;

	lookingdir = { (Getposition().x - monsterData.pos.x), (Getposition().y - monsterData.pos.y) };

	if (lookingdir.first == 0)
		lookingdir.first = 0.1f;
	if (lookingdir.second == 0)
		lookingdir.second = 0.1f;

	double temp = sqrt(pow(lookingdir.first, 2) + pow(lookingdir.second, 2));
	lookingdir.first /= -temp / OFFSETADJUST; lookingdir.second /= -temp / OFFSETADJUST;

	Setdirection({ (long)lookingdir.first,(long)lookingdir.second });

	Setangle(LookingDirToDegree(lookingdir));
	Setisactivate(true);
	Setoffset({ (long)playerData.lookingDir.first * (long)Getsize() / OFFSETADJUST / 2, (long)playerData.lookingDir.second * (long)Getsize() / OFFSETADJUST / 2 });
	Setposition({ playerData.pos.x + Getoffset().x, playerData.pos.y + Getoffset().y });
	Setmasternum(playerIndex);
}

MagicArrowSkill* magicarrowskill = nullptr;

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

	magicarrowskill->Sett2_destroy();
	double skilldestroytime = std::chrono::duration_cast<std::chrono::duration<double>>(magicarrowskill->Gett2_destroy() - magicarrowskill->Gett1_destroy()).count();

	magicarrowskill->Sett2_attacktick();
	double hittime = std::chrono::duration_cast<std::chrono::duration<double>>(magicarrowskill->Gett2_attacktick() - magicarrowskill->Gett1_attacktick()).count();

	magicarrowskill->Sett1_attacktick();

	if (skilldestroytime > TKUNAISKILLDESTROY)
	{
		OBJECTIDARR[magicarrowskill->GetID()] = false;
		delete skill;
		skill = nullptr;
	}
}