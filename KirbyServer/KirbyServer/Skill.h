#pragma once

#include "Player.h"

enum SKILLTYPE {
	KIRBY = 1,
	METAKNIGHT,
	DEDEDE,
	MABEROA,
	ELECTRICFIELD,
	KUNAI,
	MAGICARROW,
	TORNADO,
	TRUCK
};

enum COLLIDERTYPE {
	CIRCLE = 3,
	RECTANGLE
};

class Skill {
private:
	int masternum;
	int targetnum;
	int skilltype;
	int collidertype;
	int speed;
	int damage;
	int size;
	float coolTime;
	POINT position;
	POINT direction;

public:
	Skill() : masternum(0), targetnum(0), skilltype(0), collidertype(3), speed(1), damage(1), size(1), coolTime(5.0), position({0,0}), direction({0,0}) {}
	Skill(int masternum, int targetnum, int skilltype, int collidertype, int speed, int damage, int size, float coolTime, POINT position, POINT direction)
		: masternum(masternum), targetnum(targetnum), skilltype(skilltype), collidertype(collidertype),
		speed(speed), damage(damage), size(size), coolTime(coolTime), position(position), direction(direction) {}
	~Skill() {}

	int Getmasternum() { return masternum; }
	int Gettargetnum() { return targetnum; }
	int Getskilltype() { return skilltype; }
	int Getcollidertype() { return collidertype; }
	int Getspeed() { return speed; }
	int Getdamage() { return damage; }
	int Getsize() { return size; }
	float Getcooltime() { return coolTime; }
	POINT Getposition() { return position; }
	POINT Getdirection() { return direction; }

	void Setmasternum(int masternum) { this->masternum = masternum; }
	void Settargetnum(int targetnum) { this->targetnum = targetnum; }
	void Setskilltype(int skilltype) { this->skilltype = skilltype; }
	void Setcollidertype(int skilcollidertypeltype) { this->collidertype = collidertype; }
	void Setspeed(int speed) { this->speed = speed; }
	void Setdamage(int damage) { this->damage = damage; }
	void Setsize(int size) { this->size = size; }
	void Setcooltime(float coolTime) { this->coolTime = coolTime; }
	void Setposition(POINT position) { this->position = position; }
	void Setdirection(POINT direction) { this->direction = direction; }

	virtual void SetCollider(Collider2D* collider) = 0;
};