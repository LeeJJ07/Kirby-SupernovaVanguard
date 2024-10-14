#pragma once

#include "Socket.h"

class Object;

enum ColliderType
{
	TERRAIN = 0,
	PLAYER = 1,
	MONSTER = 2,
	PMISSILE = 3,
	EMISSILE = 4,
};

class Collider2D
{
private:
	POINT position;
	POINT offset;
	short type;
	bool isTrigger;

public:
	Collider2D() : position({ 0,0 }), offset({ 0,0 }), type(0), isTrigger(false) {}
	Collider2D(bool trigger, short _type) : position({ 0,0 }), offset({ 0,0 }), type(_type), isTrigger(trigger) {}
	Collider2D(POINT c, POINT _offset = { 0,0 }, short t = 0, bool trigger = false) : position(c), offset(_offset), type(t), isTrigger(trigger) {}
	~Collider2D() {}

	POINT	GetPosition()const { return position; }
	POINT	GetOffset()const { return offset; }
	short	GetType()const { return type; }
	bool	GetisTrigger()const { return isTrigger; }

	void	MovePosition(POINT targetPos) { position.x = targetPos.x + offset.x; position.y = targetPos.y + offset.y; }
	void	SetOffset(POINT newoffset) { offset = newoffset; }

	virtual void DrawCollider(HDC&) = 0;
};

extern Object** objArr;
static int objnum = 0;

template<class T>
void Create(T* t)
{
	objArr[objnum] = new T();
	objArr[objnum] = t;
	objnum++;
}