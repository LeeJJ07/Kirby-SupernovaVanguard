#pragma once

#include "Object.h"
#include "PlayerData.h"

enum ESKILLTYPE
{
	HEAL = -1,
	KIRBYSKILL = 1,
	DEDEDESKILL,
	METAKNIGHTSKILL,
	MABEROASKILL,
	ELECTRICFIELDSKILL,
	KUNAISKILL,
	MAGICARROWSKILL,
	TORNADOSKILL,
	TRUCKSKILL
};

enum ESKILLSTATE {
	ATTACKSKILL,
	DISAPPEAR
};

class Skill : public Object {
private:
	int size;
	int size2;
	float angle;
	ESKILLTYPE skillType;
	ESKILLSTATE eSkillState;

	std::map<ESKILLSTATE, Animation*> ani;
public:
	Skill() :size(0), size2(0), angle(0), skillType(KIRBYSKILL), eSkillState(ESKILLSTATE::ATTACKSKILL)
	{}
	Skill(ESKILLTYPE skillType) :size(0), size2(0), angle(0), skillType(skillType), eSkillState(ESKILLSTATE::ATTACKSKILL)
	{
		switch(skillType)
		{
		case KIRBYSKILL:
		{
			Circle2D* c = new Circle2D(true, PMISSILE);
			SetObject(c);
		}
			break;
		case DEDEDESKILL:
		{
			Circle2D* c = new Circle2D(true, PMISSILE);
			SetObject(c);
		}
			break;
		case METAKNIGHTSKILL:
		{
			Rectangle2D* rectangle = new Rectangle2D(true, PMISSILE);
			SetObject(rectangle);
		}
			break;
		case MABEROASKILL:
		{
			Circle2D* c = new Circle2D(true, PMISSILE);
			SetObject(c);
		}
			break;
		case ELECTRICFIELDSKILL:
		{
			Circle2D* c = new Circle2D(true, PMISSILE);
			SetObject(c);
		}
			break;
		case KUNAISKILL:
		{
			Rectangle2D* rectangle = new Rectangle2D(true, PMISSILE);
			SetObject(rectangle);
		}
			break;
		case MAGICARROWSKILL:
		{
			Rectangle2D* rectangle = new Rectangle2D(true, PMISSILE);
			SetObject(rectangle);
		}
			break;
		case TORNADOSKILL:
		{
			Rectangle2D* rectangle = new Rectangle2D(true, PMISSILE);
			SetObject(rectangle);
		}
			break;
		case TRUCKSKILL:
		{
			Rectangle2D* rectangle = new Rectangle2D(true, PMISSILE);
			SetObject(rectangle);
		}
			break;
		}
		SetSkillAni();
	}
	~Skill()
	{
		for (auto it = ani.begin(); it != ani.end(); it++)
			delete (*it).second;
		ani.clear();
	}

	
	int	Getangle() { return angle; }
	int	Getsize() { return size; }

	void	Setangle(int angle) { this->angle = angle; }
	void	Setsize(int size) { this->size = size; }
	void	Setsize2(int size2) { this->size2 = size2; }

	void	DrawSkill(HDC&);
	void	SetSkillAni();
	void	ObjectUpdate(TOTALDATA&, int i);
};