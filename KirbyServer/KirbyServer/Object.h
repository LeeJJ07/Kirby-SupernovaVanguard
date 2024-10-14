#pragma once
#include "Circle2D.h"

struct PLAYERDATA;

class Object
{
private:
	POINT position;
	PAIR lookingDirection;
	POINT mousePosition;

	bool isInGame;

	Collider2D* collider;
public:
	Object() :position({ 0,0 }), lookingDirection({ 1,0 }), mousePosition({ 0,0 }), isInGame(false), collider(nullptr) {}
	Object(POINT p) :position(p), lookingDirection({ 1,0 }), mousePosition({ 0,0 }), isInGame(false), collider(nullptr) {}
	~Object() { delete collider; }

	PAIR			GetLookingDir() { return lookingDirection; }
	POINT			GetMousePosition() { return mousePosition; }
	POINT			GetPosition() { return position; }
	bool			GetIsInGame() { return isInGame; }
	Collider2D*		GetCollider() { return collider; }

	void			SetPosition(POINT nPos) { position = nPos; }
	void			SetLookingDir(PAIR p) { lookingDirection = p; }
	void			SetMousePosition(POINT mousePos) { this->mousePosition = mousePos; }
	void			SetIsInGame(bool isInGame) { this->isInGame = isInGame; }
	void			SetObject(Collider2D* c) { collider = c; }

	void			ObjectUpdate(PLAYERDATA);
};