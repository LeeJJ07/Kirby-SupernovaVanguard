#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <list>
#include <vector>
#include <atlstr.h>
#include <condition_variable>
#include <chrono>
#include <iostream>
#include <string>
#include <cassert>
#include <map>
#include <fstream>
#include <sstream>

#pragma comment(lib, "msimg32.lib")

#define PAIR std::pair<double,double>
#define PLAYERNUM 4
#define MONSTERNUM 500
#define SKILLNUM 100
#define OBJECTNUM PLAYERNUM + MONSTERNUM + SKILLNUM
#define SKILLARR 6

static enum ECharacterType { KIRBY = 1, DDD, METANIHGT, MABOROA };
static enum ECharacterState { IDLE, WALK, ATTACK };
static enum ObjectImage
{
	kirby_Idle,		 kirby_Walk,		kirby_Attack,		 ddd_Idle,			ddd_Walk,			 ddd_Attack,
	meta_Idle,		 meta_Walk,			meta_Attack,		 maboroa_Idle,		maboroa_Walk,		 maboroa_Attack
};
enum OBJECTINDEX
{
	PLAYERINDEX = 0,
	MONSTERINDEX = PLAYERNUM,
	SKILLINDEX = MONSTERNUM + PLAYERNUM,
	FINALINDEX = MONSTERNUM + PLAYERNUM + SKILLNUM
};

extern short myID;

// >> : 몬스터
#define BASE_DAMAGE			10
#define BASE_HEALTH			50
#define BASE_SPEED			1	