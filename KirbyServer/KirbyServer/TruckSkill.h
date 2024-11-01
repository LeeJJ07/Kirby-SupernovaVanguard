#pragma once

#include "Skill.h"

class TruckSkill : public Skill
{
public:
	TruckSkill(
		int masternum,
		int targetnum)
		: Skill(masternum, targetnum, SKILLTYPE::TRUCKSKILL, ECOLLIDERSHAPE::RECTANGLE, 0.25, 100, 30, 150, 100, 10., { 0,0 }, { totalData.udata[masternum].pos.x, totalData.udata[masternum].pos.y }, { 5,0 })
	{
		Rectangle2D* rectangle2D = new Rectangle2D(true, PMISSILE);
		rectangle2D->SetPosition(this->Getposition());
		SetCollider(rectangle2D);
	}
	~TruckSkill()
	{   
		delete collider;
	}

	void	AssignSkill(int&, PLAYERDATA&, MONSTERDATA&);
};

void TruckSkill::AssignSkill(int& playerIndex, PLAYERDATA& playerData, MONSTERDATA& monsterData)
{
	Setoffset({ (rand() % SCREEN_SIZE_X) / 2 , (rand() % SCREEN_SIZE_Y) / 2 });
	Setposition({ playerData.pos.x + Getoffset().x, playerData.pos.y + Getoffset().y });
	
	PAIR lookingdir = { (Getposition().x - monsterData.pos.x), (Getposition().y - monsterData.pos.y) };
	double temp = sqrt(pow(lookingdir.first, 2) + pow(lookingdir.second, 2));
	lookingdir.first /= temp / OFFSETADJUST; lookingdir.second /= temp / OFFSETADJUST;

	Setdirection({ (long)(-lookingdir.first),(long)(-lookingdir.second) });

	Setangle(LookingDirToDegree(lookingdir));
	Setisactivate(true);
	Setoffset({ (long)playerData.lookingDir.first * (long)Getsize() / OFFSETADJUST / 2, (long)playerData.lookingDir.second * (long)Getsize() / OFFSETADJUST / 2 });
	Setposition({ playerData.pos.x + Getoffset().x, playerData.pos.y + Getoffset().y });
	Setmasternum(playerIndex);
}

TruckSkill* truckskill = nullptr;

void UpdateTruckSkill(Skill*& skill)
{
	truckskill = dynamic_cast<TruckSkill*>(skill);

	POINT newpos;
	newpos.x = truckskill->Getposition().x + truckskill->Getdirection().x * truckskill->Getspeed();
	newpos.y = truckskill->Getposition().y + truckskill->Getdirection().y * truckskill->Getspeed();
	truckskill->Setposition(newpos);

	truckskill->GetCollider()->SetPosition(truckskill->Getposition());
	Rectangle2D* rectangle = dynamic_cast<Rectangle2D*>(truckskill->GetCollider());
	rectangle->SetWidth(truckskill->Getsize());
	rectangle->SetHeight(truckskill->Getsize2());

	truckskill->SetCollider(rectangle);

	truckskill->Sett2_destroy();
	double skilldestroytime = std::chrono::duration_cast<std::chrono::duration<double>>(truckskill->Gett2_destroy() - truckskill->Gett1_destroy()).count();

	truckskill->Sett2_attacktick();
	double hittime = std::chrono::duration_cast<std::chrono::duration<double>>(truckskill->Gett2_attacktick() - truckskill->Gett1_attacktick()).count();

	truckskill->Sett1_attacktick();

	if (skilldestroytime > TTRUCKSKILLDESTROY)
	{
		OBJECTIDARR[truckskill->GetID()] = false;
		delete skill;
		skill = nullptr;
	}
}