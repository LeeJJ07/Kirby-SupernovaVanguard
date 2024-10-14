#pragma once
#include "Player.h"
#include "Monster.h"

#define CAMERA_WIDTH 1920
#define CAMERA_HEIGHT 1200

class Camera
{
private:
	POINT cameraPos;
	Object* targetObject;
public:
	void SetCameraPos(POINT pos) { cameraPos = pos; }
	template<class T>
	void SetTargetObject(T* target);

	POINT GetCameraPos() { return cameraPos; }

	template<class T>
	T GetTargetObject();

	void PositionUpdate();
	void RePosition();

public:
	Camera() :cameraPos({ 0,0 }), targetObject(nullptr) {}
	~Camera(){}
};

extern Camera camera;

template<class T>
inline void Camera::SetTargetObject(T* target)
{
	// >> : Monster
	{
		Monster* tObject = dynamic_cast<Monster*>(target);
		if (tObject != NULL)
		{
			targetObject = tObject;
			return;
		}
		delete tObject;
	}
	// <<

	// >> : Player
	{
		Player* tObject = dynamic_cast<Player*>(target);
		if (tObject != NULL)
		{
			targetObject = tObject;
			return;
		}

		delete targetObject;
	}
	// <<
}

template<class T>
inline T Camera::GetTargetObject()
{
	Player* tObject = dynamic_cast<Player*>(targetObject);

	if (tObject != NULL)
		return tObject;
}