#pragma once
#include "Object.h"

enum EMonsterType { RUNNER, SPEAR, WINGBUG, FIREMAN, LANDMINE, KUNGFUMAN, GAOGAO, NEBULA};
enum EMonsterState { CHASE, ATTACK, DEATH };

class Monster;

extern std::vector<Monster*> vMonster;
extern int monsterCount;

class Monster : public Object
{
private:
	EMonsterType monsterType;
	EMonsterState curState;

	int damage;
	int maxHealth;
	int curHealth;
	float speed;

	bool isEnabled;

public:
	Monster() : monsterType(RUNNER), curState(CHASE), Object()
	{
		Circle2D* c = new Circle2D(true, MONSTER);
		SetObject(c);

		this->damage = BASE_DAMAGE;
		this->maxHealth = BASE_HEALTH;
		this->curHealth = this->maxHealth;
		this->speed = BASE_SPEED;

		isEnabled = false;
	}
	Monster(POINT p) : monsterType(RUNNER), curState(CHASE), Object(p)
	{
		Circle2D* c = new Circle2D(true, MONSTER);
		SetObject(c);

		this->damage = BASE_DAMAGE;
		this->maxHealth = BASE_HEALTH;
		this->curHealth = this->maxHealth;
		this->speed = BASE_SPEED;

		isEnabled = false;
	}
	Monster(POINT p, EMonsterType mType, EMonsterState cs, int damage, int maxHealth, float speed, bool isEnabled)
		: monsterType(mType), curState(cs), Object(p)
	{
		Circle2D* c = new Circle2D(true, MONSTER);
		SetObject(c);

		this->damage = damage;
		this->maxHealth = maxHealth;
		this->curHealth = this->maxHealth;
		this->speed = speed;
		
		this->isEnabled = isEnabled;
	}

	virtual ~Monster() {}

	EMonsterType	GetMonsterType() { return monsterType; }
	void SetMosterType(EMonsterType _monsterType) { this->monsterType = _monsterType; }

	int GetDamage() { return damage; }
	int GetMaxHealth() { return maxHealth; }
	int GetCurHealth() { return curHealth; }
	float GetSpeed() { return speed; }
	void SetDamage(int damage) { this->damage = damage; }
	void SetMaxHealth(int maxHealth) { this->maxHealth = maxHealth; }
	void SetCurHealth(int curHealth) { this->curHealth = curHealth; }
	void SetSpeed(float speed) { this->speed = speed; }

	virtual void Update() = 0;
};