#include "framework.h"
#include "Camera.h"
#include "Player.h"
#include "Map.h"

void Camera::Update()
{
	if (targetObject)
	{
		SetCameraPos(targetObject->GetPos());

		Resize();
	}
}

void Camera::Resize()
{
	if (cameraPos.x < CAMERA_WIDTH / 2)
		cameraPos.x = CAMERA_WIDTH / 2;
	else if (cameraPos.x > (MAX_MAP_SIZE_X - CAMERA_WIDTH / 2))
		cameraPos.x = MAX_MAP_SIZE_X - CAMERA_WIDTH / 2;

	if (cameraPos.y < CAMERA_HEIGHT / 2)
		cameraPos.y = CAMERA_HEIGHT / 2;
	else if (cameraPos.y > (MAX_MAP_SIZE_Y - CAMERA_HEIGHT / 2))
		cameraPos.y = MAX_MAP_SIZE_Y - CAMERA_HEIGHT / 2;
}