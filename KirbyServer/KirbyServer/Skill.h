#pragma once

#include "Player.h"

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
	float coolTime;
	float speed;
	float angle;
	float size;
	float size2;
	ECOLLIDERSHAPE collidershape;
	POINT offset;
	POINT position;
	POINT direction;
public:
	Skill() :masternum(0), targetnum(0), skilltype(KIRBYSKILL), collidershape(ECOLLIDERSHAPE::CIRCLE),
		speed(1), damage(1), size(0), size2(0), coolTime(5.0), offset({0,0}), position({0,0}), direction({1,0}) {}
	Skill(int masternum, int targetnum, int skilltype, ECOLLIDERSHAPE collidershape, float speed, int damage, int size, int size2, float coolTime, POINT offset, POINT position, POINT direction)
		:masternum(masternum), targetnum(targetnum), skilltype(skilltype), collidershape(collidershape),
		speed(speed), damage(damage), size(size), size2(size2), coolTime(coolTime), offset(offset), position(position), direction(direction) {}
	~Skill() {}

	bool	Getisactivate()	{ return isactivate; }
	int	Getmasternum()	{ return masternum; }
	int Gettargetnum()	{ return targetnum; }
	int Getskilltype()	{ return skilltype; }
	int Getdamage()	{ return damage; }
	int GetID()		{ return ID; }
	float	Getcooltime() { return coolTime; }
	float	Getspeed() { return speed; }
	float	Getangle() { return angle; }
	float	Getsize() { return size; }
	float	Getsize2() { return size2; }
	ECOLLIDERSHAPE	GetcolliderShape() { return collidershape; }
	POINT	Getoffset()	{ return offset; }
	POINT	Getposition()	{ return position; }
	POINT	Getdirection()	{ return direction; }

	void	Setisactivate(bool isactivate)	{ this->isactivate = isactivate; }
	void	Setmasternum(int masternum) { this->masternum = masternum; }
	void	Settargetnum(int targetnum)	{ this->targetnum = targetnum; }
	void	Setskilltype(int skilltype)	{ this->skilltype = skilltype; }
	void	Setdamage(int damage)	{ this->damage = damage; }
	void	SetID(int ID)	{ this->ID = ID; }
	void	Setcooltime(float coolTime)	{ this->coolTime = coolTime; }
	void	Setspeed(float speed)	{ this->speed = speed; }
	void	Setangle(float angle)	{ this->angle = angle; }
	void	Setsize(float size)	{ this->size = size; }
	void	Setsize2(float size2)	{ this->size2 = size2; }
	void	Setcollidertype(ECOLLIDERSHAPE collidershape) { this->collidershape = collidershape; }
	void	Setoffset(POINT offset)	{ this->offset = offset; }
	void	Setposition(POINT position)	{ this->position = { position.x, position.y }; }
	void	Setdirection(POINT direction)	{ this->direction = direction; }

	virtual	void SetCollider(Collider2D* collider) = 0;
};

static int FindCloseMonster(POINT&);
static POINT GetNormalizationRange(POINT&, POINT&);