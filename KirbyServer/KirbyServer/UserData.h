#pragma once

#include "Player.h"

struct ObjectData
{
	PAIR lookingDir;
	POINT pos;
	POINT offset;
	int radius;
	short id;
};

struct UserData : public ObjectData
{
	POINT mousePos;
	bool inGameStart;
};

struct MonsterData :public ObjectData
{
	Player* target;

	std::chrono::high_resolution_clock::time_point t1_targeting;
	std::chrono::high_resolution_clock::time_point t2_targeting;
	std::chrono::duration<double> timeSpan_targeting;
};

void SetUserData(UserData&, Object*);
void SetObject(Object*& p, UserData& ud);