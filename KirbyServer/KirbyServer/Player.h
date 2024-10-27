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
	int	ID;
	int maxHealth =	100;
	int curHealth =	maxHealth;
	float	damage = 1.f;
	float	speed =	1.f;
	bool	isLockOn;

	std::vector<SkillManager*> vSkillManager;
public:
	Player(int ID)
		: characterType(KIRBY), Object(), ID(ID), isLockOn(true),
		damage()
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
	int	GetID()	{ return ID; }
	int GetmaxHealth()	{ return maxHealth; }
	int GetcurHealth()	{ return curHealth; }
	int Getdamage()	{ return damage; }
	int Getspeed()	{ return speed; }
	bool	GetisLockOn()	{ return isLockOn; }
	std::vector<SkillManager*> GetSkillManager() { return vSkillManager; }

	void	SetmaxHealth(int maxHealth)	{ this->maxHealth = maxHealth; }
	void	SetcurHealth(int curHealth)	{ this->curHealth = curHealth; }
	void	Setdamage(float damage)	{ this->damage = damage; }
	void	Setspeed(float speed)	{ this->speed = speed; }
	void	SetisLockOn(bool isLockOn) { this->isLockOn = isLockOn; }
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