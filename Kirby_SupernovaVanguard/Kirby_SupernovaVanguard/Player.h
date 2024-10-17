#pragma once
#include "Object.h"
#include "PlayerData.h"

enum ECharacterType;

class Player : public Object
{
private:
	ECharacterType characterType;

	POINT mousePosition;

public:
	Player() : characterType(ECharacterType::KIRBY), mousePosition({ 0,0 }), Object()
	{
		Circle2D* c = new Circle2D(true, PLAYER);
		SetObject(c);
	}

	ECharacterType	GetCharacterType() { return characterType; }
	POINT			GetMousePosition() { return mousePosition; }

	void			SetCharacterType(ECharacterType characterType) { this->characterType = characterType; }
	void			SetMousePosition(POINT mousePos) { mousePosition = mousePos; }

	void			ObjectUpdate(TOTALDATA, int i);
	void			DrawPlayer(HDC&);
};

extern std::vector<Object*> vClient;