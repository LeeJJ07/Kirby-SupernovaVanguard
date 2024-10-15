#pragma once
#include "Object.h"
#include "PlayerData.h"

enum EMonsterType { RUNNER, SPEAR, WINGBUG,  FIREMAN, LANDMINE };
enum EMonsterState { CHASE, ATTACK, DEATH };

class Monster : public Object
{
private:
	EMonsterType monsterType;

	int damage;
	int maxHealth;
	int curHealth;
	float speed;

public:
	Monster() : monsterType(RUNNER), Object()
	{
		Circle2D* c = new Circle2D(true, MONSTER);
		SetObject(c);

		this->damage = BASE_DAMAGE;
		this->maxHealth = BASE_HEALTH;
		this->curHealth = this->maxHealth;
		this->speed = BASE_SPEED;
	}
	Monster(POINT p) : monsterType(RUNNER), Object(p)
	{
		Circle2D* c = new Circle2D(true, MONSTER);
		SetObject(c);

		this->damage = BASE_DAMAGE;
		this->maxHealth = BASE_HEALTH;
		this->curHealth = this->maxHealth;
		this->speed = BASE_SPEED;

	}
	Monster(POINT p, EMonsterType a, int damage, int maxHealth, float speed)
		: monsterType(a), Object(p)
	{
		Circle2D* c = new Circle2D(true, MONSTER);
		SetObject(c);

		this->damage = damage;
		this->maxHealth = maxHealth;
		this->curHealth = this->maxHealth;
		this->speed = speed;
	}

	EMonsterType GetMonsterType() { return monsterType; }

	void SetMonsterType(EMonsterType monsterType) { this->monsterType = monsterType; }

	int GetDamage() { return damage; }
	int GetMaxHealth() { return maxHealth; }
	int GetCurHealth() { return curHealth; }
	float GetSpeed() { return speed; }
	void SetDamage(int damage) { this->damage = damage;}
	void SetMaxHealth(int maxHealth) { this->maxHealth = maxHealth;}
	void SetCurHealth(int curHealth) { this->curHealth = curHealth; }
	void SetSpeed(float speed) { this->speed = speed; }

	void OnHit(int playerDamage)
	{
		curHealth -= playerDamage;
	}

	bool IsDie()
	{
		return curHealth <= 0;
	}

	void Draw(HDC&);
	void ObjectUpdate(TOTALDATA, int i); // 이게 어떤거징?
};