#pragma once

#include "Player.h"

struct UserData
{
	PAIR lookingDir;
	POINT center;
	POINT pos;
	int radius;
	int moveDir;
	short id;
};

void SetUserData(UserData&, Player*);
void SetPlayer(Player*, UserData&);