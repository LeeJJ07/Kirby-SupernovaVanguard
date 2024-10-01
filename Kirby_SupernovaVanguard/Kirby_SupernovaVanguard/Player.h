#pragma once
#include "Circle2D.h"
#include <iostream>

#define PAIR std::pair<double,double>

class Player :public Circle2D
{
private:
	POINT pos;
	PAIR lookingDir;
	int moveDir;
public:
	Player(POINT pos = { 0,0 }, PAIR lD = { 0,0 }, int mD = 0, int centerX = 0, int centerY = 0, int r = BASERADIUS)
		:Circle2D(centerX, centerY, r), pos(pos), lookingDir(lD), moveDir(mD) {}
	~Player() {}
	int GetMoveDir() { return moveDir; }
	PAIR GetLookingDir() { return lookingDir; }
	POINT GetPos() { return pos; }
	void SetMoveDir(int d) { moveDir = d; }
	void SetLookingDir(PAIR p) { lookingDir = p; }
	void SetPos(POINT p) { pos = p; }
};