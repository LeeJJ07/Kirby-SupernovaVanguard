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
	int	maxHealth;
	int	curHealth;
	ECharacterState	curState;

	std::pair<short, int> levelUpSkillIndex[3]; //��ų Ÿ��, ���� ���� �ޱ�
};

struct MONSTERDATA :public OBJECTDATA
{
	int targetnum;
	EMonsterType monsterType;
	EMonsterState curState;
	int	maxHealth;
	int	curHealth;
};

struct SKILLDATA
{
	POINT position;
	POINT colliderPosition;
	int skillType;
	int size;
	int size2;
	int colliderShape;
	int colliderType;
	int colliderSize;
	int colliderSize2;
	int	angle;
	short id;
	short targetNum;
	char dataType;
	bool isActivate;
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
	short	targetNum;
	char dataType;
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
	PUBLICDATA		publicdata;
	PLAYERDATA		udata[PLAYERNUM];
	MONSTERDATA		mdata[MONSTERNUM];
	SKILLDATA		sdata[SKILLNUM];
	MONSTERSKILLDATA		msdata[MONSTERSKILLNUM];
<<<<<<< HEAD
	bool	send;
=======
	
	bool send;
>>>>>>> Collider
};

extern TOTALDATA uData;