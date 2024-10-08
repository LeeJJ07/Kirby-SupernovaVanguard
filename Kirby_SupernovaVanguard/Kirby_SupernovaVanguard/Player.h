#pragma once
#include "Circle2D.h"
#include <iostream>

#define PAIR std::pair<double,double>

enum ECharacterType {KIRBY, DDD, METANIHGT, MABOROA};

class Player :public Circle2D
{
private:
	PAIR lookingDir;
	int moveDir;
	POINT mousePos;

	ECharacterType characterType;

	bool isInGame;
public:
	Player(PAIR lD = { 0,0 }, int mD = 0, POINT p = { 0,0 }, POINT c = { 0,0 }, short t = PLAYER, int r = BASERADIUS)
		:Circle2D(p, c, t, r), lookingDir(lD), moveDir(mD), mousePos({0, 0}), characterType(KIRBY), isInGame(false)
	{}
	~Player() {}
	int GetMoveDir() { return moveDir; }
	PAIR GetLookingDir() { return lookingDir; }
	void SetMoveDir(int d) { moveDir = d; }
	void SetLookingDir(PAIR p) { lookingDir = p; }

	POINT GetMousePosition() { return mousePos; }
	void SetMousePosition(POINT mousePos) { this->mousePos = mousePos; }

	ECharacterType GetCharacter() { return characterType; }
	void SetCharacter(ECharacterType characterType) { this->characterType = characterType; }

	bool GetIsInGame() { return isInGame; }
	void SetIsInGame(bool isInGame) { this->isInGame = isInGame; }
};

void DrawPlayer(HDC, Player*);