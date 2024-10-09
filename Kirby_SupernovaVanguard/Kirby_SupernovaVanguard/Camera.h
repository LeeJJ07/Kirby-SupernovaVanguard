#pragma once
#include "Object.h"

class Player;

#define CAMERA_WIDTH 1920
#define CAMERA_HEIGHT 1200

class Camera
{
private:
	POINT cameraPos;
	Object* targetObject;
public:
	void SetCameraPos(POINT pos) { cameraPos = pos; }
	void SetTargetObject(Object* p) { targetObject = p; }

	POINT GetCameraPos() { return cameraPos; }
	Object GetTargetObject() { return *targetObject; }

	void PositionUpdate();
	void RePosition();

public:
	Camera() :cameraPos({ 0,0 }), targetObject(nullptr) {}
	~Camera(){}
};

extern Camera camera;