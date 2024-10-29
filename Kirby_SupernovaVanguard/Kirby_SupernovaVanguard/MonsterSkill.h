#pragma once

#include "Object.h"
#include "PlayerData.h"

enum EMONSTERSKILLTYPE {
	SPEARSKILL = 1,
	FIREMANSKILL,
	LASERSKILL,
	FIREBALLSKILL
};

enum EMONSTERSKILLSTATE {
	MATTACKSKILL,
	MDISAPPEAR
};

class MonsterSkill : public Object {
private:
	float angle;
	int size;
	int size2;

	EMONSTERSKILLTYPE monsterSkillType;
	EMONSTERSKILLSTATE eMonsterSkillState;

	std::map<EMONSTERSKILLSTATE, Animation*> monsterani;
public:
	MonsterSkill() :size(0), size2(0), angle(0), monsterSkillType(SPEARSKILL), eMonsterSkillState(EMONSTERSKILLSTATE::MATTACKSKILL)
	{}
	MonsterSkill(EMONSTERSKILLTYPE monsterSkillType) :size(0), size2(0), angle(0), monsterSkillType(monsterSkillType), eMonsterSkillState(EMONSTERSKILLSTATE::MATTACKSKILL)
	{
		switch (monsterSkillType)
		{
		case SPEARSKILL:
		{
			Rectangle2D* rectangle = new Rectangle2D(true, EMISSILE);
			SetObject(rectangle);
		}
		break;
		case FIREMANSKILL:
		{
			Circle2D* circle = new Circle2D(true, EMISSILE);
			SetObject(circle);
		}
		break;
		case LASERSKILL:
		{
			Rectangle2D* rectangle = new Rectangle2D(true, EMISSILE);
			SetObject(rectangle);
		}
		break;
		case FIREBALLSKILL:
		{
			Circle2D* circle = new Circle2D(true, EMISSILE);
			SetObject(circle);
		}
		break;
		}
		SetMonsterSkillAni();
	}
	~MonsterSkill()
	{
		for (auto it = monsterani.begin(); it != monsterani.end(); it++)
			delete (*it).second;
		monsterani.clear();
	}


	int	Getangle() { return angle; }
	int	Getsize() { return size; }

	void	Setangle(int angle) { this->angle = angle; }
	void	Setsize(int size) { this->size = size; }
	void	Setsize2(int size2) { this->size2 = size2; }

	void	DrawMonsterSkill(HDC&);
	void	SetMonsterSkillAni();
	void	ObjectUpdate(int i);
};