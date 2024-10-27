#pragma once

#include "framework.h"

class SkillManager
{
private:
	int type;
	int curLevel;
	double cooltime;

	std::chrono::high_resolution_clock::time_point t1_activate;
	std::chrono::high_resolution_clock::time_point t2_activate;
public:
	SkillManager(int type, double cooltime) : type(type), curLevel(1), cooltime(cooltime),
		t1_activate(std::chrono::high_resolution_clock::now()),
		t2_activate(std::chrono::high_resolution_clock::now())
	{}
	~SkillManager() {}

	int Gettype()		{ return type; }
	int GetCurLevel() { return curLevel; }
	double Getcooltime()	{ return cooltime; }
	std::chrono::high_resolution_clock::time_point	Gettime_1() { return t1_activate; }
	std::chrono::high_resolution_clock::time_point	Gettime_2() { return t2_activate; }

	void Settype(int type)		{ this->type	=  type; }
	void SetCurLevel(int curLevel) { this->curLevel = curLevel; }
	void Setcooltime(int colltime)	{ this->cooltime =  cooltime; }
	void Settime_1()	{ t1_activate = std::chrono::high_resolution_clock::now(); }
	void Settime_2()	{ t2_activate = std::chrono::high_resolution_clock::now(); }
};