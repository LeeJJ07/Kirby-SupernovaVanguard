// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <vector>
#include <atlstr.h>
#include <condition_variable>
#include <chrono>
#include <iostream>
#include <cmath>
#include <time.h>
#include <map>
#include <random>
#include <algorithm>

// >> file library
#include <fstream>
#include <sstream>
#include <cctype>
// <<

#define MAX_MAP_SIZE_X				4000
#define MAX_MAP_SIZE_Y				2000

#define SCREEN_SIZE_X				1920
#define SCREEN_SIZE_Y				1200

#define DEFAULT_SPAWN_SIZE_X		50
#define DEFAULT_SPAWN_SIZE_Y		50

#define FIRST_BOSS_INIT_TIME		60 //300
#define SECOND_BOSS_INIT_TIME		120 //600
#define THIRD_BOSS_INIT_TIME		180 //900

#define PAIR std::pair<double,double>
#define PLAYERNUM					4
#define MONSTERNUM					500
#define SKILLNUM					100
#define MONSTERSKILLNUM				100

#define RETARGETINGTIME				1

#define ALL_SKILL_LAST_INDEX		9

#define MIN_VALUE	9999999

enum OBJECTINDEX
{
	PLAYERINDEX = 0,
	MONSTERINDEX = PLAYERNUM,
	SKILLINDEX = MONSTERNUM + PLAYERNUM,
	MONSTERSKILLINDEX = MONSTERNUM + PLAYERNUM + SKILLNUM,
	FINALINDEX = MONSTERNUM + PLAYERNUM + SKILLNUM + MONSTERSKILLNUM
};

// >> : Monster
#define NORMAL_MONSTER_TYPE_COUNT		4

#define BASE_DAMAGE						10
#define BASE_HEALTH						100
#define BASE_SPEED						1.5	

#define RUNNER_BASE_DAMAGE				5
#define RUNNER_BASE_HEALTH				160
#define RUNNER_BASE_SPEED				1.5

#define SPEAR_BASE_DAMAGE				5
#define SPEAR_BASE_HEALTH				120
#define SPEAR_BASE_SPEED				1.5
#define SPEAR_BASE_RANGE				350

#define WINGBUG_BASE_DAMAGE				15
#define WINGBUG_BASE_HEALTH				100
#define WINGBUG_BASE_SPEED				2

#define FIREMAN_BASE_DAMAGE				8
#define FIREMAN_BASE_HEALTH				100
#define FIREMAN_BASE_SPEED				2
#define FIREMAN_BASE_RANGE				50

#define LANDMINE_BASE_DAMAGE			25
#define LANDMINE_BASE_HEALTH			5000
#define LANDMINE_BASE_SPEED				0

#define KUNGFUMAN_BASE_DAMAGE			15
#define KUNGFUMAN_BASE_HEALTH			1500
#define KUNGFUMAN_BASE_SPEED			2
#define KUNGFUMAN_BASE_ATTACK_SPEED		5
#define KUNGFUMAN_CHASE_TIME			4
#define KUNGFUMAN_ATTACK_TIME			6
#define KUNGFUMAN_DETECT_TIME			3

#define GAOGAO_BASE_DAMAGE				20
#define GAOGAO_BASE_HEALTH				1800
#define GAOGAO_BASE_ATTACK_SPEED		15
#define GAOGAO_DETECT_TIME				3
#define GAOGAO_STUN_TIME				5

#define BOSS_BASE_DAMAGE				30
#define BOSS_BASE_HEALTH				2500
#define BOSS_BASE_SPEED					0.5

// >> Skill Animation End Time
#define KIRBY_SKILL_END_TIME			0.9
#define DEDEDE_SKILL_END_TIME			0.8
#define METAKNIGHT_SKILL_END_TIME		0.5
#define MABOROA_SKILL_END_TIME			0.4

// >> Skill Destroy Time
#define TKIRBYSKILLDESTROY 2
#define TDEDEDESKILLDESTROY 2
#define TELECTRICFIELDSKILLDESTROY 3
#define TKUNAISKILLDESTROY 3
#define TMABEOROASKILLDESTROY 1
#define TMAGICARROWSKILLDESTROY 3
#define TMETAKNIGHTSKILLDESTROY 1
#define TTORNADOSKILLDESTROY 4
#define TTRUCKSKILLDESTROY 6

// >> MonsterSkill Destroy Time
#define TSPEARSKILLDESTROY 3
#define TFIREMANSKILLDESTROY 1
#define TLASERSKILLDESTROY 4
#define TFIREBALLSKILLDESTROY 3

// >> Skill offset
#define MAXLEVEL	5
#define STATNUM	7
#define OFFSETADJUST 50