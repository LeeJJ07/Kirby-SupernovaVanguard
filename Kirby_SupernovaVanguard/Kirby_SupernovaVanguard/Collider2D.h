#pragma once

#include "Socket.h"

class Player;

enum ColliderType
{
	TERRAIN		= 0,
	PLAYER		= 1,
	MONSTER		= 2,
	PMISSILE	= 3,
	EMISSILE	= 4,
};

enum ColliderFigureType
{
	CIRCLEFIGURE = 3,
	RECTANGLEFIGURE = 4,
};

class Collider2D
{
private:
	POINT position;
	POINT offset;
	ColliderType type;
	int angle;
	bool isTrigger;

public:
	Collider2D() : position({ 0,0 }), offset({ 0,0 }), type(TERRAIN), isTrigger(false) {}
	Collider2D(bool trigger, ColliderType _type) : position({ 0,0 }), offset({ 0,0 }), type(_type), isTrigger(trigger) {}
	Collider2D(POINT c, POINT _offset = { 0,0 }, ColliderType t = TERRAIN, bool trigger = false) : position(c), offset(_offset), type(t), isTrigger(trigger) {}
	~Collider2D() {}

	POINT	GetPosition()const { return position; }
	POINT	GetOffset()const { return offset; }
	int		Getangle() { return angle; }
	short	GetType()const { return type; }
	bool	GetisTrigger()const { return isTrigger; }

	void	MovePosition(POINT targetPos) { position.x = targetPos.x + offset.x; position.y = targetPos.y + offset.y; }
	void	SetOffset(POINT newoffset) { offset = newoffset; }
	void	Setangle(int angle) { this->angle = angle; }

	virtual void DrawCollider(HDC&, int& ID) = 0;
};

extern Object** objArr;

template<class T>
void CreateObject(T* t, int ID)
{
	objArr[ID] = t;
}