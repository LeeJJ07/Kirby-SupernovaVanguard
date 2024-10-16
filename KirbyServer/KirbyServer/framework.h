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

#define SCREEN_SIZE_X 1920
#define SCREEN_SIZE_Y 1200

#define DEFAULT_SPAWN_SIZE_X 50
#define DEFAULT_SPAWN_SIZE_Y 50

#define PAIR std::pair<double,double>
#define PLAYERNUM					4
#define MONSTERNUM					1000
#define RETARGETINGTIME				1


// >> : Monster
#define BASE_DAMAGE					10
#define BASE_HEALTH					50
#define BASE_SPEED					1	

#define NORMAL_MONSTER_TYPE_COUNT	5

#define RUNNER_BASE_DAMAGE			20
#define RUNNER_BASE_HEALTH			50
#define RUNNER_BASE_SPEED			2

#define SPEAR_BASE_DAMAGE			7
#define SPEAR_BASE_HEALTH			30
#define SPEAR_BASE_SPEED			1

#define WINGBUG_BASE_DAMAGE			10
#define WINGBUG_BASE_HEALTH			20
#define WINGBUG_BASE_SPEED			2.5

#define FIREMAN_BASE_DAMAGE			30
#define FIREMAN_BASE_HEALTH			15
#define FIREMAN_BASE_SPEED			2

#define LANDMINE_BASE_DAMAGE		30
#define LANDMINE_BASE_HEALTH		150
#define LANDMINE_BASE_SPEED			0