#pragma once

#include "Player.h"
#include "TotalData.h"

extern struct SKILLDATA;
extern struct MONSTERDATA;
class Monster;

#define KIRBYTICK	0.1f
#define DEDEDETICK	0.5f
#define METAKNIGHTTICK	0.1f
#define MABEROATICK	0.05f
#define ELECTRICFIELDTICK	0.8f
#define KUNAITICK	1.f
#define MAGICARROWTICK	0.5f
#define TORNADOTICK	0.4f
#define TRUCKTICK	0.5f

enum SKILLTYPE {
	KIRBYSKILL = 1,
	DEDEDESKILL,
	METAKNIGHTSKILL,
	MABEROASKILL,
	ELECTRICFIELDSKILL,
	KUNAISKILL,
	MAGICARROWSKILL,
	TORNADOSKILL,
	TRUCKSKILL
};

class Skill {
private:
	bool isactivate = false;
	int masternum;
	int targetnum;
	int skilltype;
	int damage;
	int ID = -1;
	int pierceCount;
	int amount;
	int curLevel;
	float coolTime;
	float speed;
	float angle;
	float size;
	float size2;
	ECOLLIDERSHAPE collidershape;
	POINT offset;
	POINT position;
	POINT direction;

	std::chrono::high_resolution_clock::time_point t1_attacktick;
	std::chrono::high_resolution_clock::time_point t2_attacktick;

	std::chrono::high_resolution_clock::time_point t1_coolTime;
	std::chrono::high_resolution_clock::time_point t2_coolTime;

	std::chrono::high_resolution_clock::time_point t1_destroy;
	std::chrono::high_resolution_clock::time_point t2_destroy;
protected:
	Collider2D* collider;
public:
	Skill() :masternum(0), targetnum(0), skilltype(KIRBYSKILL), collidershape(ECOLLIDERSHAPE::CIRCLE),
		speed(1), damage(1), size(0), size2(0), amount(1), curLevel(1), coolTime(5.0), offset({0,0}), position({0,0}), direction({1,0}), pierceCount(1) {}
	Skill(int masternum, int targetnum, int skilltype, ECOLLIDERSHAPE collidershape, float speed, int damage, int pierceCount,
		int size, int size2, float coolTime, POINT offset, POINT position, POINT direction)
		:masternum(masternum), targetnum(targetnum), skilltype(skilltype), collidershape(collidershape), pierceCount(pierceCount),
		speed(speed), damage(damage), size(size), size2(size2), amount(1), curLevel(1), coolTime(coolTime), offset(offset), position(position), direction(direction)
	{
		Sett1_destroy();
	}
	~Skill()
	{
		vHitMonster.clear(); // 벡터를 비워줌
	}

	std::vector< std::pair<Monster*, std::chrono::duration<float>>> vHitMonster;

	bool	Getisactivate()	{ return isactivate; }
	int	Getmasternum()	{ return masternum; }
	int Gettargetnum()	{ return targetnum; }
	int Getskilltype()	{ return skilltype; }
	int Getdamage()	{ return damage; }
	int GetID()		{ return ID; }
	int GetpierceCount()		{ return pierceCount; }
	int GetAmount() { return amount; }
	int GetCurLevel() { return curLevel; }
	float	Getcooltime() { return coolTime; }
	float	Getspeed() { return speed; }
	float	Getangle() { return angle; }
	float	Getsize() { return size; }
	float	Getsize2() { return size2; }
	ECOLLIDERSHAPE	GetcolliderShape() { return collidershape; }
	POINT	Getoffset()	{ return offset; }

	POINT	Getposition()	{ return position; }
	POINT	Getdirection()	{ return direction; }
	std::chrono::high_resolution_clock::time_point Gett1_attacktick() { return t1_attacktick; }
	std::chrono::high_resolution_clock::time_point Gett2_attacktick() { return t2_attacktick; }
	std::chrono::high_resolution_clock::time_point Gett1_coolTime() { return t1_coolTime; }
	std::chrono::high_resolution_clock::time_point Gett2_coolTime() { return t2_coolTime; }
	std::chrono::high_resolution_clock::time_point Gett1_destroy() { return t1_destroy; }
	std::chrono::high_resolution_clock::time_point Gett2_destroy() { return t2_destroy; }

	Collider2D* GetCollider() { return collider; }

	void	Setisactivate(bool isactivate)	{ this->isactivate = isactivate; }
	void	Setmasternum(int masternum) { this->masternum = masternum; }
	void	Settargetnum(int targetnum)	{ this->targetnum = targetnum; }
	void	Setskilltype(int skilltype)	{ this->skilltype = skilltype; }
	void	Setdamage(int damage)	{ this->damage = damage; }
	void	SetID(int ID)	{ this->ID = ID; }
	void	SetpierceCount(int pierceCount)	{ this->pierceCount = pierceCount; }
	void	SetAmount(int amount) { this->amount = amount; }
	void	SetCurLevel(int curLevel) { this->curLevel = curLevel; }
	void	Setcooltime(float coolTime)	{ this->coolTime = coolTime; }
	void	Setspeed(float speed)	{ this->speed = speed; }
	void	Setangle(float angle)	{ this->angle = angle; }
	void	Setsize(float size)	{ this->size = size; }
	void	Setsize2(float size2)	{ this->size2 = size2; }
	void	Setcollidertype(ECOLLIDERSHAPE collidershape) { this->collidershape = collidershape; }
	void	Setoffset(POINT offset)	{ this->offset = offset; }
	void	Setposition(POINT position)	{ this->position = { position.x, position.y }; }
	void	Setdirection(POINT direction)	{ this->direction = direction; }
	void	SetCollider(Collider2D* collider) { this->collider = collider; }

	void	Sett1_attacktick() { t1_attacktick = std::chrono::high_resolution_clock::now(); }
	void	Sett2_attacktick() { t2_attacktick = std::chrono::high_resolution_clock::now(); }
	void	Sett1_coolTime() { t1_coolTime = std::chrono::high_resolution_clock::now(); }
	void	Sett2_coolTime() { t2_coolTime = std::chrono::high_resolution_clock::now(); }
	void	Sett1_destroy() { t1_destroy = std::chrono::high_resolution_clock::now(); }
	void	Sett2_destroy() { t2_destroy = std::chrono::high_resolution_clock::now(); }

	void	SetSkillInDataSheet(SKILLDATA&);
	virtual void	AssignSkill(int&, PLAYERDATA&, MONSTERDATA&);

	void	Update(float deltaTime)
	{
		for (auto it = vHitMonster.begin(); it != vHitMonster.end();) {
			it->second -= std::chrono::duration<float>(deltaTime);
			if (it->second.count() <= 0.0f) {
				it = vHitMonster.erase(it);
			}
			else {
				++it;
			}
		}
	}
	virtual void	UpdateSkill() {}
};

static int FindCloseMonster(POINT&);
static POINT GetNormalizationRange(POINT&, POINT&);

static float LookingDirToDegree(PAIR& lookingdir)
{
	double angleRadians = atan2(lookingdir.second, lookingdir.first);

	// 라디안 값을 도(degree)로 변환하려면
	double angleDegrees = angleRadians * 180.0 / 3.14;

	if (angleDegrees < 0)
	{
		angleDegrees = 180 + (180 - abs(angleDegrees));
	}

	return angleDegrees;
}