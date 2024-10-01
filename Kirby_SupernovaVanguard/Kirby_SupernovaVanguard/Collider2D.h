#pragma once

#include "Socket.h"

class Collider2D
{
private:
	POINT center;
public:
	Collider2D(int x = 0, int y = 0) :center({ x,y }) {}
	~Collider2D() {}

	void MoveCenter(POINT m) { center.x += m.x; center.y += m.y; }
	void SetCenter(POINT p) { center.x = p.x; center.y = p.y; }
	POINT GetCenter()const { return center; }
};