#pragma once

#include "Object.h"

struct UserData
{
	bool inGameStart;

	PAIR lookingDir;
	POINT center;
	POINT pos;
	POINT mousePos;
	int radius;
	int moveDir;
	short id;
};

void SetUserData(UserData&, Player*);
void SetPlayer(std::vector<Player*>& p, UserData&);
void SetPlayer(Player*& p, UserData& ud);