#pragma once

#include "Animation.h"
#include "Camera.h"

enum EUITYPE
{
	GAMEOVER,
	RESTART
};

class UI
{
	EUITYPE uiType;
	std::map<EUITYPE, Animation* > ani;
public:
	UI(EUITYPE uiType) :uiType(uiType) {}
	~UI()
	{
		for (auto it = ani.begin(); it != ani.end(); it++)
			delete (*it).second;
		ani.clear();
	}

	std::map<EUITYPE, Animation* > Getani() { return ani; }

	void	SetUIAni();
	void	DrawUI(HDC&, Camera&);
};