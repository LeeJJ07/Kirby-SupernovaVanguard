#pragma once
#include "Circle2D.h"
#include "Rectangle2D.h"

class PLAYERDATA;
class MONSTERDATA;
class Collider2D;
class Animation;

class Object
{
protected:
	POINT position;
	PAIR lookingDirection;

	bool isInGame;
	
	Animation* curAni;
	Collider2D* collider;
public:
	Object() :position({ 0,0 }), lookingDirection({ 1,0 }), isInGame(false), collider(nullptr), curAni(nullptr)
	{
	
	}
	Object(POINT p) :position(p), lookingDirection({ 1,0 }), isInGame(false), collider(nullptr), curAni(nullptr) {}
	~Object() { delete collider; delete curAni; }

	PAIR			GetLookingDir() { return lookingDirection; }
	POINT			GetPosition() { return position; }
	bool			GetIsInGame() { return isInGame; }
	Animation*		GetAnimation() { return curAni; }
	Collider2D*		GetCollider() { return collider; }


	void			SetPosition(POINT nPos) { position = nPos; }
	void			SetLookingDir(PAIR p) { lookingDirection = p; }
	void			SetIsInGame(bool _isInGame) { isInGame = _isInGame; }
	void			SetObject(Collider2D* c) { collider = c; }
	void			SetAnimation(Animation* ani) { curAni = ani; }

	virtual void	ObjectUpdate(TOTALDATA&, int i) = 0;
};