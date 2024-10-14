#pragma once

#include "Player.h"
#include "Monster.h"

struct OBJECTDATA
{
	char dataType;
	PAIR lookingDir;
	POINT pos;
	POINT offset;
	int radius;
	short id;
};

struct PLAYERDATA :public OBJECTDATA
{
	POINT mousePos;
	bool inGameStart;
};

struct MONSTERDATA :public OBJECTDATA
{
	Player* target;
	enum EMonsterType monstertype;

	std::chrono::high_resolution_clock::time_point t1_targeting;
	std::chrono::high_resolution_clock::time_point t2_targeting;
	std::chrono::duration<double> timeSpan_targeting;
};

struct TOTALDATA
{
	PLAYERDATA		udata[PLAYERNUM] = {};
	MONSTERDATA		mdata[MONSTERNUM];
};

void SetObjectData(TOTALDATA*&, Object*);
void SetObject(Object*& p, TOTALDATA*& ud);