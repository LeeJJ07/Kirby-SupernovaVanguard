#pragma once
#include "Circle2D.h"
#include <iostream>

#define PAIR std::pair<double,double>

enum ECharacterType {KIRBY, DDD, METANIHGT, MABOROA};

class Object
{
private:
	POINT position;
	PAIR lookingDirection;
	POINT mousePosition;

	ECharacterType characterType;

	bool isInGame;

	Collider2D* collider;
public:
	Object() :position({ 0,0 }), lookingDirection({ 1,0 }), mousePosition({ 0,0 }), characterType(KIRBY), isInGame(false)
	{
		collider = nullptr;
	}
	Object(POINT pos = { 0,0 }, PAIR lookDir = { 1,0 }, POINT p = { 0,0 })
		: position(pos), lookingDirection(lookDir), mousePosition({ 0, 0 }), characterType(KIRBY), isInGame(false)
	{
		collider = new Circle2D(true, PLAYER);
	}
	~Object() { delete collider; }

	PAIR			GetLookingDir() { return lookingDirection; }
	POINT			GetMousePosition() { return mousePosition; }
	ECharacterType	GetCharacter() { return characterType; }
	bool			GetIsInGame() { return isInGame; }
	Collider2D*		GetCollider() { return collider; }

	void			SetLookingDir(PAIR p) { lookingDirection = p; }
	void			SetMousePosition(POINT mousePos) { this->mousePosition = mousePos; }
	void			SetCharacter(ECharacterType characterType) { this->characterType = characterType; }
	void			SetIsInGame(bool isInGame) { this->isInGame = isInGame; }
};

void DrawPlayer(HDC, Player*);