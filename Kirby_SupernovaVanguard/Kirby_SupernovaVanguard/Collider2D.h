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

enum ColliderShape
{
	CIRCLE = 3,
	RECTANGLE = 4,
};

class Collider2D
{
private:
	POINT position;
	POINT offset;
	ColliderType type;
	ColliderShape shape;
	int angle;
	bool isTrigger;

public:
	Collider2D(ColliderShape shape)
		: position({ 0,0 }), offset({ 0,0 }), type(TERRAIN), isTrigger(false) {}
	Collider2D(bool trigger, ColliderType type, ColliderShape shape)
		: position({ 0,0 }), offset({ 0,0 }), type(type), shape(shape), isTrigger(trigger) {}
	Collider2D(POINT c, ColliderShape shape, POINT _offset = { 0,0 }, ColliderType type = TERRAIN, bool trigger = false)
		: position(c), offset(_offset), shape(shape), type(type), isTrigger(trigger) {}
	~Collider2D() {}

	POINT	GetPosition()const { return position; }
	POINT	GetOffset()const { return offset; }
	int		Getangle() { return angle; }
	short	GetColliderShape()const { return shape; }
	short	GetColliderType()const { return type; }
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