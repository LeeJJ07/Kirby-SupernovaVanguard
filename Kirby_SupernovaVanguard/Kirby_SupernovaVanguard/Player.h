#pragma once
#include "Circle2D.h"
#include <iostream>

#define PAIR std::pair<double,double>

class Player :public Circle2D
{
private:
	PAIR lookingDir;
	int moveDir;
public:
	Player(PAIR lD = { 0,0 }, int mD = 0, POINT p = { 0,0 }, POINT c = { 0,0 }, short t = PLAYER, int r = BASERADIUS)
		:Circle2D(p, c, t, r), lookingDir(lD), moveDir(mD) {}
	~Player() {}
	int GetMoveDir() { return moveDir; }
	PAIR GetLookingDir() { return lookingDir; }
	void SetMoveDir(int d) { moveDir = d; }
	void SetLookingDir(PAIR p) { lookingDir = p; }
};

void DrawPlayer(HDC, Player*);