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

#define SCREEN_SIZE_X				1920
#define SCREEN_SIZE_Y				1200

#define PAIR std::pair<double,double>
#define PLAYERNUM 4
#define MONSTERNUM 500
#define SKILLNUM 100
#define MONSTERSKILLNUM 100
#define OBJECTNUM PLAYERNUM + MONSTERNUM + SKILLNUM + MONSTERSKILLNUM
#define SKILLARR 6

static enum ECharacterType { KIRBY = 1, DDD, METANIHGT, MABOROA };
static enum ECharacterState { IDLE, WALK, ATTACK };

//ObjectImage enum 데이터와 txt 파일의 데이터 순서가 일치해야함.
static enum ObjectImage
{
	kirby_Idle,		 kirby_Walk,		kirby_Attack,		 ddd_Idle,			ddd_Walk,			 ddd_Attack,
	meta_Idle,		 meta_Walk,			meta_Attack,		 maboroa_Idle,		maboroa_Walk,		 maboroa_Attack,
	runner_Walk,	 spear_Walk,		spear_Attack,		 wingbug_Walk,		fireman_Walk,		 fireman_Attack,
	landmine_Idle,	 KFM_Attack,		gaogao_Attack,		 boss_eye,			boss_mode1,			 boss_mode2,
	boss_Attack1,	 boss_Attack2,		boss_Attack3
};

enum OBJECTINDEX
{
	PLAYERINDEX = 0,
	MONSTERINDEX = PLAYERNUM,
	SKILLINDEX = MONSTERNUM + PLAYERNUM,
	MONSTERSKILLINDEX = MONSTERNUM + PLAYERNUM + SKILLNUM,
	FINALINDEX = MONSTERNUM + PLAYERNUM + SKILLNUM + MONSTERSKILLNUM
};

extern short myID;

// >> : 몬스터
#define BASE_DAMAGE			10
#define BASE_HEALTH			50
#define BASE_SPEED			1	