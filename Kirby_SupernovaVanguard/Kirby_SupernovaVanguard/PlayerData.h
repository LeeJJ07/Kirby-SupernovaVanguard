#pragma once

#include "Player.h"
//#include "Monster.h"

enum EMonsterType;
enum EMonsterState;
#include "Monster.h"
#include "Skill.h"

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
	char charactertype;
};

struct SKILLDATA
{
	bool isactivate;
	short id;
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
	EMonsterType monsterType;
	EMonsterState curState;
};

struct TOTALDATA
{
	float nowTime;
	PLAYERDATA		udata[PLAYERNUM];
	MONSTERDATA		mdata[MONSTERNUM];
	SKILLDATA		sdata[SKILLNUM];
};

void SetObjectData(TOTALDATA*&, Object*);
void SetObject(Object*& p, TOTALDATA*& ud);