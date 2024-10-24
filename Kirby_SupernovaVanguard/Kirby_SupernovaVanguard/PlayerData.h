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
	POINT position;
	POINT colliderPosition;
	int skillType;
	int size;
	int size2;
	int colliderType;
	int colliderSize;
	int colliderSize2;
	int	angle;
	short id;
	short targetNum;
	bool isActivate;
};

struct MONSTERSKILLDATA
{
	POINT	position;
	POINT	colliderPosition;
	int skillType;
	int size;
	int size2;
	int colliderType;
	int	colliderSize;
	int	colliderSize2;
	int angle;
	short	id;
	short	targetNum;
	bool	isActivate;
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
	MONSTERSKILLDATA		msdata[MONSTERSKILLNUM];
};

void SetObjectData(TOTALDATA*&, Object*);
void SetObject(Object*& p, TOTALDATA*& ud);