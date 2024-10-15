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

	std::vector<SkillManager*> vSkillManager;
public:
	Player() : characterType(ECharacterType::KIRBY), Object()
	{
		Circle2D* c = new Circle2D(true, PLAYER);
		SetObject(c);
	}

	ECharacterType	GetCharacterType() { return characterType; }
	void			SetCharacterType(ECharacterType characterType) { this->characterType = characterType; }
	void			DrawPlayer(HDC&);

	std::vector<SkillManager*> GetSkillManager() { return vSkillManager; }
};

extern std::vector<Player*> vClient;