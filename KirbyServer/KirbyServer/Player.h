#pragma once
#include "Object.h"
#include "Skill.h"
#include "SkillManager.h"

enum ECharacterType
{
	KIRBY = 1,
	DEDEDE,
	METAKNIGHT,
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
	~Player()
	{
		for (auto vsm : vSkillManager)
			delete vsm;
	}

	ECharacterType	GetCharacterType() { return characterType; }
	int	GetID() { return ID; }
	std::vector<SkillManager*> GetSkillManager() { return vSkillManager; }

	void	SetSkillManager(std::vector<SkillManager*> vSkillManager) { this->vSkillManager = vSkillManager; }

	void	SetCharacterType(int characterType)
	{
		switch (characterType)
		{
		case 1:
			this->characterType = KIRBY;
			break;
		case 2:
			this->characterType = DEDEDE;
			break;
		case 3:
			this->characterType = METAKNIGHT;
			break;
		case 4:
			this->characterType = MABOROA;
			break;
		}
	}
	void	DrawPlayer(HDC&);
};

extern std::vector<Player*> vClient;