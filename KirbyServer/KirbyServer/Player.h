#pragma once
#include "Object.h"
#include "Skill.h"
#include "SkillManager.h"

enum ECharacterType
{
	KIRBY = 1,
	METANIHGT,
	DDD,
	MABOROA
};

class Player : public Object
{
private:
	ECharacterType characterType;
	int ID;

	std::vector<SkillManager*> vSkillManager;
public:
	Player(int ID) : characterType(KIRBY), Object(), ID(ID)
	{
		Circle2D* c = new Circle2D(true, PLAYER);
		SetObject(c);
	}

	ECharacterType	GetCharacterType() { return characterType; }
	int	GetID() { return ID; }
	std::vector<SkillManager*> GetSkillManager() { return vSkillManager; }

	void	SetCharacterType(ECharacterType characterType) { this->characterType = characterType; }
	void	DrawPlayer(HDC&);
};

extern std::vector<Player*> vClient;