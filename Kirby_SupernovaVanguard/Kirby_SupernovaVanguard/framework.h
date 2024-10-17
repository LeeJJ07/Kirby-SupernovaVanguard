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

#pragma comment(lib, "msimg32.lib")

#define PAIR std::pair<double,double>
#define PLAYERNUM 4
#define MONSTERNUM 100
#define SKILLNUM 100
#define OBJECTNUM 2000
#define SKILLARR 6

static enum ECharacterType { KIRBY, DDD, METANIHGT, MABEOROA };

extern short myID;

// >> : 몬스터
#define BASE_DAMAGE			10
#define BASE_HEALTH			50
#define BASE_SPEED			1	