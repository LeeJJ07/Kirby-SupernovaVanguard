#pragma once
#include "Object.h"

enum EMonsterType { RUNNER, SPEAR, WINGBUG, FIREMAN, LANDMINE, KUNGFUMAN, GAOGAO, NEBULA};
enum EMonsterState { CHASE, ATTACK, DEATH };

class Monster;

extern std::vector<Monster*> vMonster;
extern int monsterCount;

class Monster : public Object
{
protected:
	EMonsterType monsterType;
	EMonsterState curState;

	POINT targetPos;

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

		this->targetPos = { 0, 0 };
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

		this->targetPos = { 0, 0 };
		this->damage = BASE_DAMAGE;
		this->maxHealth = BASE_HEALTH;
		this->curHealth = this->maxHealth;
		this->speed = BASE_SPEED;

		isEnabled = false;
	}
	Monster(POINT p, EMonsterType mType, EMonsterState cs, POINT targetPos, int damage, int maxHealth, float speed, bool isEnabled)
		: monsterType(mType), curState(cs), Object(p)
	{
		Circle2D* c = new Circle2D(true, MONSTER);
		SetObject(c);

		this->targetPos = targetPos;
		this->damage = damage;
		this->maxHealth = maxHealth;
		this->curHealth = this->maxHealth;
		this->speed = speed;
		
		this->isEnabled = isEnabled;
	}

	virtual ~Monster() {}

	EMonsterType	GetMonsterType() { return monsterType; }
	void SetMosterType(EMonsterType monsterType) { this->monsterType = monsterType; }
	EMonsterState GetMonsterState() { return curState; }
	void SetMonsterState(EMonsterState nextState) { this->curState = nextState; }

	POINT GetTargetPos() { return targetPos; }
	int GetDamage() { return damage; }
	int GetMaxHealth() { return maxHealth; }
	int GetCurHealth() { return curHealth; }
	float GetSpeed() { return speed; }
	void SetTargetPos(POINT targetPos) { this->targetPos = targetPos; }
	void SetDamage(int damage) { this->damage = damage; }
	void SetMaxHealth(int maxHealth) { this->maxHealth = maxHealth; }
	void SetCurHealth(int curHealth) { this->curHealth = curHealth; }
	void SetSpeed(float speed) { this->speed = speed; }

	virtual void Update() = 0;
};