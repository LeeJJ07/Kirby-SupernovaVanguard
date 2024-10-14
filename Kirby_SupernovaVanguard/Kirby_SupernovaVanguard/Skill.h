#pragma once

#include "Player.h"

class Skill {
private:
	float coolTime;
	int speed;
	int damage;
	int size;
	Player* master;
	POINT position;
	POINT direction;

public:
	Skill() :coolTime(5.0), speed(1), damage(1), size(1), master(nullptr), position({ 0,0 }), direction({ 0,0 }) {}
	~Skill() {}

	float Getcooltime() { return coolTime; }
	int Getspeed() { return speed; }
	int Getdamage() { return damage; }
	int Getsize() { return size; }
	Player* Getmaster() { return master; }
	POINT Getposition() { return position; }
	POINT Getdirection() { return direction; }

	void Setcooltime(float coolTime) { this->coolTime = coolTime; }
	void Setspeed(int speed) { this->speed = speed; }
	void Setdamage(int damage) { this->damage = damage; }
	void Setsize(int size) { this->size = size; }
	void Setmaster(Player* master) { this->master = master; }
	void Setposition(POINT position) { this->position = position; }
	void Setdirection(POINT direction) { this->direction = direction; }
};