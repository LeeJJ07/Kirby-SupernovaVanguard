#pragma once

#include "Socket.h"

class Object;

enum ECOLLIDERTYPE
{
	TERRAIN = 0,
	PLAYER = 1,
	MONSTER = 2,
	PMISSILE = 3,
	EMISSILE = 4,
};

enum ECOLLIDERSHAPE {
	CIRCLE = 3,
	RECTANGLE
};

class Collider2D
{
private:
	POINT position;
	POINT offset;
	ECOLLIDERTYPE	type;
	ECOLLIDERSHAPE	shape;
	bool isTrigger;

public:
	Collider2D(ECOLLIDERSHAPE shape) : position({ 0,0 }), offset({ 0,0 }), type(TERRAIN), isTrigger(false),shape(shape) {}

	Collider2D(bool trigger, ECOLLIDERTYPE _type, ECOLLIDERSHAPE shape)
		: position({ 0,0 }), offset({ 0,0 }), type(_type), isTrigger(trigger), shape(shape)
	{}

	Collider2D(POINT c, ECOLLIDERSHAPE shape, POINT _offset = { 0,0 }, ECOLLIDERTYPE t = TERRAIN, bool trigger = false)
		: position(c), offset(_offset), type(t), isTrigger(trigger), shape(shape)
	{}
	~Collider2D() {}

	POINT	GetPosition()const { return position; }
	POINT	GetOffset()const { return offset; }
	short	GetType()const { return type; }
	bool	GetisTrigger()const { return isTrigger; }

	void	MovePosition(POINT targetPos) { position.x = targetPos.x + offset.x; position.y = targetPos.y + offset.y; }
	void	SetOffset(POINT newoffset) { offset = newoffset; }
	void	SetPosition(POINT position) { this->position = position; }

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