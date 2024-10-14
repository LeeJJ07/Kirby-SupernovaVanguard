#pragma once
#include "Object.h"
#include "PlayerData.h"

enum ECharacterType { KIRBY, DDD, METANIHGT, MABEOROA };

class Player : public Object
{
private:
	ECharacterType characterType;

	POINT mousePosition;

public:
	Player() : characterType(KIRBY), mousePosition({ 0,0 }), Object()
	{
		Circle2D* c = new Circle2D(true, PLAYER);
		SetObject(c);
	}

	ECharacterType	GetCharacterType() { return characterType; }
	POINT			GetMousePosition() { return mousePosition; }

	void			SetCharacterType(ECharacterType characterType) { this->characterType = characterType; }
	void			SetMousePosition(POINT mousePos) { mousePosition = mousePos; }

	void			ObjectUpdate(PLAYERDATA);
	void			DrawPlayer(HDC&);
};

extern std::vector<Object*> vClient;