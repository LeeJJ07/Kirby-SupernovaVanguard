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
	POINT center;
	POINT offset;
	short type;
	bool isTrigger;



public:
	Collider2D() : center({ 0,0 }), offset({ 0,0 }), type(0), isTrigger(false) {}
	Collider2D(bool trigger, short _type) : center({ 0,0 }), offset({ 0,0 }), type(_type), isTrigger(trigger) {}
	Collider2D(POINT c, POINT _offset = { 0,0 }, short t = 0, bool trigger = false) : center(c), offset(_offset), type(t), isTrigger(trigger) {}
	~Collider2D() {}

	POINT GetCenter()const { return center; }
	short GetType()const { return type; }
	bool GetisTrigger()const { return isTrigger; }

	void MoveCenter(POINT target) { center.x = target.x + offset.x; center.y = target.y + offset.y; }
	void SetOffset(POINT newoffset) { offset = newoffset; }

	virtual void DrawCollider(HDC) = 0;
	void Update();
};

extern Collider2D** objArr;
static int objnum = 0;

template<class T>
void Create(T* t)
{
	objArr[objnum] = new T();
	objArr[objnum] = t;
	objnum++;
}