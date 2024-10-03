#pragma once

#include "Socket.h"

enum ColliderType
{
	TERRAIN = 0,
	PLAYER = 1,
	ENEMY = 2,
	PMISSILE = 3,
	EMISSILE = 4,
};

class Collider2D
{
private:
	POINT pos;
	POINT center;
	short type;
public:
	Collider2D(POINT p = { 0,0 }, POINT c = { 0,0 }, short t = 0) :pos(p), center(c), type(t) {}
	~Collider2D() {}

	POINT GetPos() { return pos; }
	POINT GetCenter()const { return center; }
	short GetType()const { return type; }

	void SetCenter(POINT p) { center.x = p.x; center.y = p.y; }
	void SetType(short t) { type = t; }
	void SetPos(POINT p) { pos = p; }

	void MoveCenter(POINT m) { center.x += m.x; center.y += m.y; }
};

static Collider2D** objArr;
static int objnum = 0;

template<class T>
void Create(T* t)
{
	objArr[objnum] = new T();
	objArr[objnum] = t;
	objnum++;
}