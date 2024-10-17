#pragma once

#include "Object.h"
#include "PlayerData.h"

static int skillID = 0;

enum SKILLTYPE {
	KIRBYSKILL = 1,
	METAKNIGHTSKILL,
	DEDEDESKILL,
	MABEROASKILL,
	ELECTRICFIELDSKILL,
	KUNAISKILL,
	MAGICARROWSKILL,
	TORNADOSKILL,
	TRUCKSKILL
};

class Skill : public Object {
private:
	int size;
<<<<<<< HEAD
	SKILLTYPE skilltype;

public:
	Skill() :size(0),skilltype(KIRBYSKILL)
	{}
	~Skill() {}

	
	int	Getsize() { return size; }
	SKILLTYPE	Getskilltype() { return skilltype; }

	void	Setsize(int size) { this->size = size; }
	void	Setskilltype(int i)
	{
		switch (i)
		{
		case KIRBYSKILL:
			this->skilltype = KIRBYSKILL;
			break;
		}
	}

	void	ObjectUpdate(TOTALDATA, int i);
=======
	int id;
	Object* master;
	POINT position;
	POINT direction;

public:
	Skill() :coolTime(5.0), speed(1), damage(1), size(1), id(skillID++), master(nullptr), position({0,0}), direction({0,0}) {}
	~Skill() {}

	float Getcooltime() { return coolTime; }
	int Getspeed() { return speed; }
	int Getdamage() { return damage; }
	int Getsize() { return size; }
	int GetID() { return id; }
	Object* Getmaster() { return master; }
	POINT Getposition() { return position; }
	POINT Getdirection() { return direction; }

	void Setcooltime(float coolTime) { this->coolTime = coolTime; }
	void Setspeed(int speed) { this->speed = speed; }
	void Setdamage(int damage) { this->damage = damage; }
	void Setsize(int size) { this->size = size; }
	void SetID(int id) { this->id = id; }
	void Setmaster(Object* master) { this->master = master; }
	void Setposition(POINT position) { this->position = position; }
	void Setdirection(POINT direction) { this->direction = direction; }
>>>>>>> 272e4e3653de8eaf007019700fa50379e3a0c918
};