#pragma once

#include "Player.h"

struct PlayerData {
	bool inGameStart;

	PAIR lookingDir;
	POINT pos;
	POINT offset;
	POINT mousePos;
	int radius;
	short id;
};

void SetUserData(PlayerData&, Player*);
void SetObject(Player*& p, PlayerData& ud);