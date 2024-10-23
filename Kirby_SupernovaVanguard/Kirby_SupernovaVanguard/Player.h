#pragma once
#include "Object.h"
#include "PlayerData.h"
#include "Animation.h"


extern std::map<ObjectImage, Animation*> imageDatas;

class Animation;

class Player : public Object
{
private:
	ECharacterType characterType;
	ECharacterState characterState;

	POINT mousePosition;
	std::map<ECharacterState, Animation* > ani;
public:
	Player() : characterType(KIRBY),characterState(IDLE), mousePosition({ 0,0 }), Object()
	{
		Circle2D* c = new Circle2D(true, PLAYER);
		SetObject(c);
	}
	~Player()
	{
		for (auto it = ani.begin(); it != ani.end(); it++)
			delete (*it).second;
		ani.clear();
	}

	ECharacterType	GetCharacterType() { return characterType; }
	POINT			GetMousePosition() { return mousePosition; }

	void			SetCharacterType(ECharacterType characterType) { this->characterType = characterType; }
	void			SetMousePosition(POINT mousePos) { mousePosition = mousePos; }

	void			ObjectUpdate(TOTALDATA&, int i);
	void			SetPlayerAni();
	void			DrawPlayer(HDC&);
};

extern std::vector<Object*> vClient;