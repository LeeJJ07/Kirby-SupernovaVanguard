#pragma once

#include "Player.h"
#include "Monster.h"
#include "RunnerMonster.h"
#include "SpearMonster.h"
#include "WingBugMonster.h"
#include "FireManMonster.h"
#include "LandMineMonster.h"
#include "KungFuMan.h"
#include "GaoGao.h"
#include "Boss.h"

enum DATATYPE {
	PLAYERTYPE = 'p',
	MONSTERTYPE = 'm',
	ITEMTYPE = 'i'
};

struct OBJECTDATA
{
	char dataType;
	PAIR lookingDir;
	POINT pos;
	POINT offset;
	int radius;
	short id;
};

struct PLAYERDATA : public OBJECTDATA
{
	POINT mousePos;
	bool inGameStart;
};

struct MONSTERDATA :public OBJECTDATA
{
	int targetnum;
	EMonsterType monsterType;

	std::chrono::high_resolution_clock::time_point t1_targeting;
	std::chrono::high_resolution_clock::time_point t2_targeting;
	std::chrono::duration<double> timeSpan_targeting;
};

struct TOTALDATA
{
	PLAYERDATA		udata[PLAYERNUM];
	MONSTERDATA		mdata[MONSTERNUM];
};

void SetUserData(PLAYERDATA&, Object*);
void SetObject(Object*& p, PLAYERDATA& ud);
void SetTarget(MONSTERDATA& mData, TOTALDATA& tData);