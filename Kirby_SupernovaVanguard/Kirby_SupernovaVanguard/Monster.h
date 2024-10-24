#pragma once
#include "Object.h"
#include "PlayerData.h"

enum EMonsterType { RUNNER, SPEAR, WINGBUG,  FIREMAN, LANDMINE, KUNGFUMAN, GAOGAO, BOSS };
enum EMonsterState { MONSTER_CHASE, MONSTER_ATTACK, MONSTER_DEATH, MONSTER_IDLE, MONSTER_DETECT, MONSTER_STUN };

class Monster : public Object
{
protected:
	EMonsterType monsterType;
	EMonsterState curState;

	int damage;
	int maxHealth;
	int curHealth;
	float speed;

	std::map<EMonsterState, Animation*> ani;

public:
	Monster() : monsterType(RUNNER), curState(MONSTER_CHASE), Object()
	{
		Circle2D* c = new Circle2D(true, MONSTER);
		SetObject(c);

		this->damage = BASE_DAMAGE;
		this->maxHealth = BASE_HEALTH;
		this->curHealth = this->maxHealth;
		this->speed = BASE_SPEED;
	}
	Monster(POINT p) : monsterType(RUNNER), curState(MONSTER_CHASE), Object(p)
	{
		Circle2D* c = new Circle2D(true, MONSTER);
		SetObject(c);

		this->damage = BASE_DAMAGE;
		this->maxHealth = BASE_HEALTH;
		this->curHealth = this->maxHealth;
		this->speed = BASE_SPEED;
	}
	Monster(EMonsterType type) : monsterType(type), curState(MONSTER_CHASE), Object()
	{
		Circle2D* c = new Circle2D(true, MONSTER);
		SetObject(c);

		this->damage = BASE_DAMAGE;
		this->maxHealth = BASE_HEALTH;
		this->curHealth = this->maxHealth;
		this->speed = BASE_SPEED;

		SetMonsterAni();
	}
	Monster(POINT p, EMonsterType a, EMonsterState s, int damage, int maxHealth, float speed)
		: monsterType(a), curState(s), Object(p)
	{
		Circle2D* c = new Circle2D(true, MONSTER);
		SetObject(c);

		this->damage = damage;
		this->maxHealth = maxHealth;
		this->curHealth = this->maxHealth;
		this->speed = speed;
	}
	~Monster()
	{
		for (auto it = ani.begin(); it != ani.end(); it++)
			delete (*it).second;
		ani.clear();
	}

	EMonsterType GetMonsterType() { return monsterType; }
	EMonsterState GetMonsterState() { return curState; }

	void SetMonsterType(EMonsterType monsterType) { this->monsterType = monsterType; }
	void SetMonsterState(EMonsterState nextState) { this->curState = nextState; }

	int GetDamage() { return damage; }
	int GetMaxHealth() { return maxHealth; }
	int GetCurHealth() { return curHealth; }
	float GetSpeed() { return speed; }
	void SetDamage(int damage) { this->damage = damage;}
	void SetMaxHealth(int maxHealth) { this->maxHealth = maxHealth;}
	void SetCurHealth(int curHealth) { this->curHealth = curHealth; }
	void SetSpeed(float speed) { this->speed = speed; }
	void SetLookingDir(PAIR lookingDir) { this->lookingDirection = lookingDir; }
	void SetMonsterAni();

	void OnHit(int playerDamage)
	{
		curHealth -= playerDamage;
	}

	bool IsDie()
	{
		return curHealth <= 0;
	}

	void Draw(HDC&);
	void ObjectUpdate(TOTALDATA&, int i);
};