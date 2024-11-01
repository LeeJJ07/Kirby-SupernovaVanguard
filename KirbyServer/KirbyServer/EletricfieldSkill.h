#pragma once

#include "Skill.h"

class ElectricfieldSkill : public Skill
{
public:
	ElectricfieldSkill(
		int masternum,
		int targetnum,
		int radius)
		: Skill(masternum, targetnum, SKILLTYPE::ELECTRICFIELDSKILL, ECOLLIDERSHAPE::CIRCLE, 0, 5, 1000, 80, 0, 1.2, { 0,0 }, { totalData.udata[masternum].pos.x, totalData.udata[masternum].pos.y }, { 5,0 })
	{
		Circle2D* circle2D = new Circle2D(true, PMISSILE, radius);
		circle2D->SetPosition(this->Getposition());
		SetCollider(circle2D);
	}
	~ElectricfieldSkill()
	{
		delete collider;
	}

	void	AssignSkill(int&, PLAYERDATA&, MONSTERDATA&);
};

void ElectricfieldSkill::AssignSkill(int& playerIndex, PLAYERDATA& playerData, MONSTERDATA& monsterData)
{
	Setdirection({ 0,0 });
	Setisactivate(true);
	Setoffset({ 0, 0 });
	Setposition({ playerData.pos.x + Getoffset().x, playerData.pos.y + Getoffset().y });
	Setmasternum(playerIndex);
}

ElectricfieldSkill* electricfieldskill = nullptr;

void UpdateElectricfieldSkill(Skill*& skill)
{
	electricfieldskill = dynamic_cast<ElectricfieldSkill*>(skill);

	POINT newpos;
	newpos.x = totalData.udata[electricfieldskill->Getmasternum()].pos.x + electricfieldskill->Getoffset().x;
	newpos.y = totalData.udata[electricfieldskill->Getmasternum()].pos.y + electricfieldskill->Getoffset().y;
	electricfieldskill->Setposition(newpos);

	electricfieldskill->GetCollider()->SetPosition(electricfieldskill->Getposition());
	Circle2D* circle = dynamic_cast<Circle2D*>(electricfieldskill->GetCollider());
	circle->SetRadius(electricfieldskill->Getsize());

	electricfieldskill->SetCollider(circle);

	electricfieldskill->Sett1_destroy();
	double skilldestroytime = std::chrono::duration_cast<std::chrono::duration<double>>(electricfieldskill->Gett2_destroy() - electricfieldskill->Gett1_destroy()).count();

	electricfieldskill->Sett2_attacktick();
	double hittime = std::chrono::duration_cast<std::chrono::duration<double>>(electricfieldskill->Gett2_attacktick() - electricfieldskill->Gett1_attacktick()).count();

	electricfieldskill->Sett1_attacktick();
}