#pragma once
#include "Player.h"

#define CAMERA_WIDTH 1920
#define CAMERA_HEIGHT 1200

class Camera
{
private:
	POINT cameraPos;
	Player* targetObject;
public:
	void SetCameraPos(POINT pos) { cameraPos = pos; }
	void SetTargetObject(Player* p) { targetObject = p; }

	POINT GetCameraPos() { return cameraPos; }
	Player GetTargetObject() { return *targetObject; }

	void PositionUpdate();
	void RePosition();

public:
	Camera() :cameraPos({ 0,0 }), targetObject(nullptr) {}
	~Camera(){}
};

extern Camera camera;