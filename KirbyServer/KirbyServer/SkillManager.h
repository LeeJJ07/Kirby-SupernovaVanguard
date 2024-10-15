#pragma once

#include "framework.h"

class SkillManager
{
private:
	int type;
	double cooltime;

	std::chrono::high_resolution_clock::time_point t1_activate;
	std::chrono::high_resolution_clock::time_point t2_activate;
public:
	SkillManager(int type, double cooltime) : type(type),cooltime(cooltime),
		t1_activate(std::chrono::high_resolution_clock::now()),
		t2_activate(std::chrono::high_resolution_clock::now())
	{}
	~SkillManager() {}

	int Gettype()		{ return type; }
	int Getcooltime()	{ return cooltime; }
	std::chrono::high_resolution_clock::time_point	Gettime_1() { return t1_activate; }
	std::chrono::high_resolution_clock::time_point	Gettime_2() { return t2_activate; }

	int Settype()		{ this->type	=  type; }
	int Setcooltime()	{ this->cooltime=  cooltime; }
	void Settime_1()	{ t1_activate = std::chrono::high_resolution_clock::now(); }
	void Settime_2()	{ t2_activate = std::chrono::high_resolution_clock::now(); }
};