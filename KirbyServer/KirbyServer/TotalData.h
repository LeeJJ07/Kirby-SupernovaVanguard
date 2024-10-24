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
	SKILLTYPE = 's',
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
	short	targetnum;
	char	dataType;
	bool	isActivate;
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
	short	targetnum;
	char	dataType;
	bool	isActivate;
};

struct MONSTERDATA :public OBJECTDATA
{
	int	targetnum;
	EMonsterType	monsterType;
	EMonsterState	curState;
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
	PUBLICDATA			publicdata;
	PLAYERDATA			udata[PLAYERNUM];
	MONSTERDATA			mdata[MONSTERNUM];
	SKILLDATA			sdata[SKILLNUM];
	MONSTERSKILLDATA	msdata[MONSTERSKILLNUM];
};

static TOTALDATA totalData;

void SetUserData(PLAYERDATA&, Object*);
void SetObject(Object*& p, PLAYERDATA& ud);