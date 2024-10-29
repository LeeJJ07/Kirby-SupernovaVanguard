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
	POINT	mousePos;
	bool	inGameStart;
	bool	isAlive;
	char	charactertype;
	int	maxHealth;
	int	curHealth;
	ECharacterState	curState;

	std::pair<short, int> levelUpSkillIndex[3]; //스킬 타입, 현재 레벨 담아서 전송
};

struct MONSTERDATA :public OBJECTDATA
{
	int	targetnum;
	EMonsterType	monsterType;
	EMonsterState	curState;
	int	maxHealth;
	int	curHealth;
};

struct SKILLDATA
{
	POINT	position;
	POINT	colliderPosition;
	int skillType;
	int size;
	int size2;
	int colliderShape;
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
	int colliderShape;
	int colliderType;
	int	colliderSize;
	int	colliderSize2;
	int angle;
	short	id;
	short	targetnum;
	char	dataType;
	bool	isActivate;
};

struct PUBLICDATA
{
	int		isOK;
	bool	isAllPlayerChoice;
	float	currentTime;
	int		level;
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
	bool	send;
};

static TOTALDATA totalData;