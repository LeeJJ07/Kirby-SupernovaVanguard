#pragma once
#include "Circle2D.h"

struct PLAYERDATA;

class Object
{
private:
	POINT	position;
	POINT	mousePosition;
	PAIR	lookingDirection;

	bool isInGame;

	Collider2D* collider;
public:
	Object() :position({ 0,0 }), mousePosition({ 0,0 }), lookingDirection({ 1,0 }), isInGame(false), collider(nullptr) {}
	Object(POINT p) :position(p), mousePosition({ 0,0 }), lookingDirection({ 1,0 }), isInGame(false), collider(nullptr) {}
	~Object() { delete collider; }

	POINT			GetPosition()	{ return position; }
	POINT			GetMousePosition()	{ return mousePosition; }
	PAIR			GetLookingDir()	{ return lookingDirection; }
	bool			GetIsInGame()	{ return isInGame; }
	Collider2D*		GetCollider()	{ return collider; }

	void			SetPosition(POINT nPos)	{ position = nPos; }
	void			SetMousePosition(POINT mousePos)	{ this->mousePosition = mousePos; }
	void			SetLookingDir(PAIR p)	{ lookingDirection = p; }
	void			SetIsInGame(bool isInGame)	{ this->isInGame = isInGame; }
	void			SetObject(Collider2D* c)	{ collider = c; }

	void			ObjectUpdate(PLAYERDATA);
};