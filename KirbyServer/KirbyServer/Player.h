#pragma once
#include "Object.h"
#include "Skill.h"
#include "SkillManager.h"

enum ECharacterState { PIDLE, PWALK, PATTACK, PDEATH };

enum ECharacterType
{
	KIRBY = 1,
	DEDEDE,
	METAKNIGHT,
	MABOROA
};

enum ECharacterSize
{
	KIRBYSIZE = 10,
	DEDEDESIZE = 15,
	METAKNIGHTSIZE = 10,
	MABOROASIZE = 12,
};

class Player : public Object
{
private:
	ECharacterType characterType;
	int	ID;
	int maxHealth =	100;
	int curHealth =	maxHealth;
	int playerSize;
	int	killCount = 0;
	int special = 0;
	float	damage = 1.f;
	float	speed =	1.f;
	bool	isLockOn;
	bool	isAlive = true;

	SkillManager* vSkillManager;
	int allSkillLevel[10];
public:
	Player(int ID, int playerSize, int radius)
		: characterType(KIRBY), Object(), ID(ID), playerSize(playerSize),isLockOn(true),
		damage()
	{
		Circle2D* c = new Circle2D(true, PLAYER, radius);
		SetObject(c);
		for (int i = 0; i < 10; i++)
			allSkillLevel[i] = 0;
		this->vSkillManager = new SkillManager();
	}
	~Player()
	{
			delete vSkillManager;
	}

	ECharacterType	GetCharacterType() { return characterType; }
	int	GetID()	{ return ID; }
	int GetmaxHealth()	{ return maxHealth; }
	int GetcurHealth()	{ return curHealth; }
	int Getdamage()	{ return damage; }
	int Getspeed()	{ return speed; }
	int GetSkillLevel(int idx) { return allSkillLevel[idx]; }
	int GetplayerSize() { return playerSize; }
	int	GetkillCount()	{ return killCount; }
	int	Getspecial()	{ return special; }
	bool	GetisLockOn()	{ return isLockOn; }
	bool	GetisAlive()	{ return isAlive; }
	SkillManager*& GetSkillManager() { return vSkillManager; }

	void	SetmaxHealth(int maxHealth)	{ this->maxHealth = maxHealth; }
	void	SetcurHealth(int curHealth)	{ this->curHealth = curHealth; }
	void	SetplayerSize(int playerSize)	{ this->playerSize = playerSize; }
	void	SetkillCount(int killCount)	{ this->killCount = killCount; }
	void	Setspecial(int special)	{ this->special = special; }
	void	PluskillCount()	{ killCount++; }
	void	Plusspecial()	{ special++; }
	void	Setdamage(float damage)	{ this->damage = damage; }
	void	Setspeed(float speed)	{ this->speed = speed; }
	void	SetisLockOn(bool isLockOn)	{ this->isLockOn = isLockOn; }
	void	SetisAlive(bool isAlive)	{ this->isAlive = isAlive; }
	void	SetSkillManager(SkillManager* vSkillManager)	{ this->vSkillManager = vSkillManager; }
	void	SetSkillLevel(int idx, int level)	{ allSkillLevel[idx] = level; }
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