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

struct PLAYERDATA :public OBJECTDATA
{
	POINT mousePos;
	bool inGameStart;
	int charactertype;
};

struct SKILLDATA
{
	bool isactivate;
	short id;
	int skilltype;
	int size;
	int size2;
	int collidertype;
	int collidersize;
	int collidersize2;
	POINT position;
	POINT colliderposition;
};

struct MONSTERDATA :public OBJECTDATA
{
	int targetnum;
	EMonsterType monsterType;
	EMonsterState curState;
};

struct PUBLICDATA
{
	float	currentTime;
	int		exp;
	int		maxExp;
	bool	islevelUp;
};

struct TOTALDATA
{
	PUBLICDATA		publicdata;
	PLAYERDATA		udata[PLAYERNUM];
	MONSTERDATA		mdata[MONSTERNUM];
	SKILLDATA		sdata[SKILLNUM];
};

static TOTALDATA totalData;

void SetUserData(PLAYERDATA&, Object*);
void SetObject(Object*& p, PLAYERDATA& ud);
void SetTarget(MONSTERDATA& mData, TOTALDATA& tData);