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
#include <gdiplus.h>
#include <tchar.h>
#pragma comment (lib,"Gdiplus.lib")
#pragma comment(lib, "msimg32.lib")

#define SCREEN_SIZE_X				1920
#define SCREEN_SIZE_Y				1200

#define SKILL_SELECTOR_WIDTH		500
#define SKILL_SELECTOR_HEIGHT		500
#define SKILL_SELECTOR_TITLE_WIDTH  1000
#define SKILL_SELECTOR_TITLE_HEIGHT  300


#define PAIR std::pair<double,double>
#define PLAYERNUM			4
#define MONSTERNUM			500
#define SKILLNUM			100
#define MONSTERSKILLNUM		100
#define OBJECTNUM PLAYERNUM + MONSTERNUM + SKILLNUM + MONSTERSKILLNUM
#define SKILLARR 6

static enum ECharacterType { KIRBY = 1, DDD, METANIHGT, MABOROA };
static enum ECharacterState { IDLE, WALK, ATTACK };

//ObjectImage enum 데이터와 txt 파일의 데이터 순서가 일치해야함.
static enum ObjectImage
{
	kirby_Idle,			kirby_Walk,				kirby_Attack,			ddd_Idle,				ddd_Walk,				ddd_Attack,
	meta_Idle,			meta_Walk,				meta_Attack,			maboroa_Idle,			maboroa_Walk,			maboroa_Attack,
	runner_Walk,		spear_Walk,				spear_Attack,			wingbug_Walk,			fireman_Walk,			fireman_Attack,
	landmine_Idle,		KFM_Walk,				KFM_Attack,				KFM_Detect,				gaogao_Attack,			gaogao_Detect,
	gaogao_Stun,		boss_eye,				boss_mode1,				boss_mode2,				boss_Attack1,			boss_Attack2,		 
	boss_Attack3,		kirbySkill_Attack,		dededeSkill_Attack,		metaknightSkill_Attack,	maberoaSkill_Attack,	electricfieldSkill_Attack,
	kunaiSkill_Attack,	magicarrowSkill_Attack, tornadoSkill_Attack,	truckSkill_Attack,		spearSkill_Attack,
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