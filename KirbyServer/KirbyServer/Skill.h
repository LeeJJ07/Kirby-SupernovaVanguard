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

enum COLLIDERTYPE {
	CIRCLE = 3,
	RECTANGLE
};

class Skill {
private:
	bool isactivate = false;
	int masternum;
	int targetnum;
	int skilltype;
	int collidertype;
	int speed;
	int damage;
	int size;
	int ID = -1;
	float coolTime;
	POINT offset;
	POINT position;
	POINT direction;
public:
	Skill() :masternum(0), targetnum(0), skilltype(KIRBYSKILL), collidertype(COLLIDERTYPE::CIRCLE),
		speed(1), damage(1), size(1), coolTime(5.0), offset({ 0,0 }), position({ 0,0 }), direction({ 1,0 }) {}
	Skill(int masternum, int targetnum, int skilltype, int collidertype, int speed, int damage, int size, float coolTime, POINT offset, POINT position, POINT direction)
		:masternum(masternum), targetnum(targetnum), skilltype(skilltype), collidertype(collidertype),
		speed(speed), damage(damage), size(size), coolTime(coolTime), offset(offset), position(position), direction(direction) {}
	~Skill() {}

	bool	Getisactivate()	{ return isactivate; }
	int	Getmasternum()	{ return masternum; }
	int Gettargetnum()	{ return targetnum; }
	int Getskilltype()	{ return skilltype; }
	int Getcollidertype()	{ return collidertype; }
	int Getspeed()	{ return speed; }
	int Getdamage()	{ return damage; }
	int Getsize()	{ return size; }
	int GetID()		{ return ID; }
	float	Getcooltime() { return coolTime; }
	POINT	Getoffset()	{ return offset; }
	POINT	Getposition()	{ return position; }
	POINT	Getdirection()	{ return direction; }

	void	Setisactivate(bool isactivate)	{ this->isactivate = isactivate; }
	void	Setmasternum(int masternum) { this->masternum = masternum; }
	void	Settargetnum(int targetnum) { this->targetnum = targetnum; }
	void	Setskilltype(int skilltype) { this->skilltype = skilltype; }
	void	Setcollidertype(int collidertype) { this->collidertype = collidertype; }
	void	Setspeed(int speed)	{ this->speed = speed; }
	void	Setdamage(int damage)	{ this->damage = damage; }
	void	Setsize(int size)	{ this->size = size; }
	void	SetID(int ID)		{ this->ID = ID; }
	void	Setcooltime(float coolTime)	{ this->coolTime = coolTime; }
	void	Setoffset(POINT offset) { this->offset = offset; }
	void	Setposition(POINT position) { this->position = { position.x, position.y }; }
	void	Setdirection(POINT direction)	{ this->direction = direction; }

	virtual	void SetCollider(Collider2D* collider) = 0;
};