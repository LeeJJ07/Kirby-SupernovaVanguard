#pragma once
#include "Object.h"
#include "PlayerData.h"
#include "Animation.h"
#include "ActionData.h"

class Animation;

class Player : public Object
{
private:
	ECharacterType characterType;
	ECharacterState characterState;
	int maxHealth;
	int curHealth;
	int playerSize;
	int	killCount = 0;
	int special = 0;

	POINT mousePosition;
	std::map<ECharacterState, Animation* > ani;
public:
	Player() : characterType(KIRBY), characterState(IDLE), mousePosition({ 0,0 }), Object()
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

	ECharacterType	GetCharacterType()	{ return characterType; }
	ECharacterState	GetCharacterState()	{ return characterState; }
	POINT	GetMousePosition()	{ return mousePosition; }
	int		GetmaxHealth()	{ return maxHealth; }
	int		GetcurHealth()	{ return curHealth; }
	int		GetplayerSize()	{ return playerSize; }
	int		GetkillCount()	{ return killCount; }
	int		Getspecial()	{ return special; }
	std::map<ECharacterState, Animation* > Getani() { return ani; }

	void	SetCharacterType(ECharacterType characterType)	{ this->characterType = characterType; }
	void	SetCharacterState(ECharacterState characterState)	{ this->characterState = characterState; }
	void	SetMousePosition(POINT mousePos)	{ mousePosition = mousePos; }
	void	SetmaxHealth(int maxHealth)	{ this->maxHealth = maxHealth; }
	void	SetcurHealth(int curHealth)	{ this->curHealth = curHealth; }
	void	SetplayerSize(int playerSize)	{ this->playerSize = playerSize; }
	void	SetkillCount(int killCount)	{ this->killCount = killCount; }
	void	Setspecial(int special)	{ this->special = special; }

	void	ObjectUpdate(int i, TOTALDATA& uData);
	void	SetPlayerAni();
	void	DrawPlayer(HDC&, ActionData&);
};

extern std::vector<Object*> vClient;