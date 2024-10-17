#pragma once

#include "Monster.h"

class Monster;

class Boss: public Monster
{
private:




public:
	void StateUpdate() override;
	void Update() override;
};

