#pragma once
#include "Object.h"
#include "MonsterSkill.h"
#include "MonsterSkillManager.h"

enum EMonsterType { RUNNER, SPEAR, WINGBUG, FIREMAN, LANDMINE, KUNGFUMAN, GAOGAO, BOSS};
enum EMonsterState { CHASE, ATTACK, DEATH, IDLE, DETECT, STUN };

class Monster;

extern std::vector<Monster*> vMonster;
extern int monsterCount;

static std::vector<Monster*> monsterArr(MONSTERNUM);

class Monster : public Object
{
protected:
	EMonsterType monsterType;
	EMonsterState curState;

	POINT targetPos;

	int damage;
	int maxHealth;
	int curHealth;
	int expValue = 50;
	float speed;

	bool isEnabled;

	std::chrono::high_resolution_clock::time_point t1_targeting;
	std::chrono::high_resolution_clock::time_point t2_targeting;

	MonsterSkillManager* vMonsterSkillManager;
public:
	Monster() : monsterType(RUNNER), curState(CHASE), Object()
	{
		Circle2D* c = new Circle2D(true, MONSTER, 10);
		SetObject(c);

		this->targetPos = { 0, 0 };
		this->damage = BASE_DAMAGE;
		this->maxHealth = BASE_HEALTH;
		this->curHealth = this->maxHealth;
		this->speed = BASE_SPEED;

		isEnabled = false;

		vMonsterSkillManager = new MonsterSkillManager;
	}
	Monster(POINT p, int radius) : monsterType(RUNNER), curState(CHASE), Object(p)
	{
		Circle2D* c = new Circle2D(true, MONSTER, radius);
		SetObject(c);

		this->targetPos = { 0, 0 };
		this->damage = BASE_DAMAGE;
		this->maxHealth = BASE_HEALTH;
		this->curHealth = this->maxHealth;
		this->speed = BASE_SPEED;

		isEnabled = false;

		vMonsterSkillManager = new MonsterSkillManager;
	}
	Monster(POINT p, int radius, EMonsterType mType, EMonsterState cs, POINT targetPos, int damage, int maxHealth, float speed, bool isEnabled)
		: monsterType(mType), curState(cs), Object(p)
	{
		Circle2D* c = new Circle2D(true, MONSTER, radius);
		SetObject(c);

		this->targetPos = targetPos;
		this->damage = damage;
		this->maxHealth = maxHealth;
		this->curHealth = this->maxHealth;
		this->speed = speed;
		
		this->isEnabled = isEnabled;

		vMonsterSkillManager = new MonsterSkillManager;
	}

	virtual ~Monster()
	{
		delete vMonsterSkillManager;
	}

	EMonsterType	GetMonsterType() { return monsterType; }
	void SetMosterType(EMonsterType monsterType) { this->monsterType = monsterType; }
	EMonsterState GetMonsterState() { return curState; }
	void SetMonsterState(EMonsterState nextState) { this->curState = nextState; }

	POINT	GetTargetPos() { return targetPos; }
	int		GetDamage() { return damage; }
	int		GetmaxHealth() { return maxHealth; }
	int		GetcurHealth() { return curHealth; }
	int		GetexpValue() { return expValue; }
	float	GetSpeed() { return speed; }
	bool	GetEnabled() { return isEnabled; }
	std::chrono::high_resolution_clock::time_point Gett1_targeting() { return t1_targeting; }
	std::chrono::high_resolution_clock::time_point Gett2_targeting() { return t2_targeting; }
	MonsterSkillManager*& GetMonsterSkillManager() { return vMonsterSkillManager; }

	void	SetTargetPos(POINT targetPos) { this->targetPos = targetPos; }
	void	SetDamage(int damage) { this->damage = damage; }
	void	SetmaxHealth(int maxHealth) { this->maxHealth = maxHealth; }
	void	SetcurHealth(int curHealth) { this->curHealth = curHealth; }
	void	SetexpValue(int expValue) { this->expValue = expValue; }
	void	SetSpeed(float speed) { this->speed = speed; }
	void	SetEnabled(bool isEnabled) { this->isEnabled = isEnabled; }
	void	Sett1_targeting() { t1_targeting = std::chrono::high_resolution_clock::now(); }
	void	Sett2_targeting() { t2_targeting = std::chrono::high_resolution_clock::now(); }
	void	SetMonsterSkillManager(MonsterSkillManager* vMonsterSkillManager) { this->vMonsterSkillManager = vMonsterSkillManager; }

	virtual void StateUpdate() = 0;
	virtual void Update() = 0;
};