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

#define FIRST_BOSS_INIT_TIME		30 //300
#define SECOND_BOSS_INIT_TIME		60 //600
#define THIRD_BOSS_INIT_TIME		90 //900

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
#define BASE_HEALTH						50
#define BASE_SPEED						1.5	

#define RUNNER_BASE_DAMAGE				20
#define RUNNER_BASE_HEALTH				50
#define RUNNER_BASE_SPEED				2

#define SPEAR_BASE_DAMAGE				7
#define SPEAR_BASE_HEALTH				30
#define SPEAR_BASE_SPEED				1.5
#define SPEAR_BASE_RANGE				300

#define WINGBUG_BASE_DAMAGE				10
#define WINGBUG_BASE_HEALTH				20
#define WINGBUG_BASE_SPEED				2.5

#define FIREMAN_BASE_DAMAGE				30
#define FIREMAN_BASE_HEALTH				15
#define FIREMAN_BASE_SPEED				2
#define FIREMAN_BASE_RANGE				50

#define LANDMINE_BASE_DAMAGE			30
#define LANDMINE_BASE_HEALTH			150
#define LANDMINE_BASE_SPEED				0

#define KUNGFUMAN_BASE_DAMAGE			70
#define KUNGFUMAN_BASE_HEALTH			1000
#define KUNGFUMAN_BASE_SPEED			1.5
#define KUNGFUMAN_BASE_ATTACK_SPEED		4.5
#define KUNGFUMAN_CHASE_TIME			3
#define KUNGFUMAN_ATTACK_TIME			6
#define KUNGFUMAN_DETECT_TIME			2

#define GAOGAO_BASE_DAMAGE				100
#define GAOGAO_BASE_HEALTH				500
#define GAOGAO_BASE_ATTACK_SPEED		12
#define GAOGAO_DETECT_TIME				2
#define GAOGAO_STUN_TIME				3

#define BOSS_BASE_DAMAGE				30
#define BOSS_BASE_HEALTH				150
#define BOSS_BASE_SPEED					1

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