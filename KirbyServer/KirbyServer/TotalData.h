#pragma once

#include "Player.h"
#include "Monster.h"

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
	int skilltype;
	int size;
	int collidertype;
	int collidersize;
	POINT position;
	POINT colliderposition;
};

struct MONSTERDATA :public OBJECTDATA
{
	int targetnum;
	EMonsterType monstertype;

	std::chrono::high_resolution_clock::time_point t1_targeting;
	std::chrono::high_resolution_clock::time_point t2_targeting;
	std::chrono::duration<double> timeSpan_targeting;
};

struct TOTALDATA
{
	PLAYERDATA		udata[PLAYERNUM];
	MONSTERDATA		mdata[MONSTERNUM];
	SKILLDATA		sdata[SKILLNUM];
};

static TOTALDATA totalData;

void SetUserData(PLAYERDATA&, Object*);
void SetObject(Object*& p, PLAYERDATA& ud);
void SetTarget(MONSTERDATA& mData, TOTALDATA& tData);