#pragma once

#include "Player.h"

struct UserData
{
	bool inGameStart;

	PAIR lookingDir;
	POINT pos;
	POINT offset;
	POINT mousePos;
	int radius;
	short id;
};

void SetUserData(UserData&, Object*);
void SetObject(Object*& p, UserData& ud);