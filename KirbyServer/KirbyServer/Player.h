#pragma once
#include "Object.h"

enum ECharacterType { KIRBY, DDD, METANIHGT, MABOROA };

class Player : public Object
{
private:
	ECharacterType characterType;

public:
	Player() : characterType(KIRBY), Object()
	{
		Circle2D* c = new Circle2D(true, PLAYER);
		SetObject(c);
	}

	ECharacterType	GetCharacterType() { return characterType; }
	void			SetCharacterType(ECharacterType characterType) { this->characterType = characterType; }
	void			DrawPlayer(HDC&);
};

extern std::vector<Player*> vClient;