#pragma once

#include "Object.h"
#include "PlayerData.h"

static int skillID = 0;

enum SKILLTYPE {
	KIRBYSKILL = 1,
	DEDEDESKILL,
	METAKNIGHTSKILL,
	MABEROASKILL,
	ELECTRICFIELDSKILL,
	KUNAISKILL,
	MAGICARROWSKILL,
	TORNADOSKILL,
	TRUCKSKILL
};

class Skill : public Object {
private:
	int size;
	int size2;
	float angle;
	SKILLTYPE skilltype;

public:
	Skill() :size(0),size2(0),skilltype(KIRBYSKILL)
	{}
	~Skill() {}

	
	int	Getangle() { return angle; }
	int	Getsize() { return size; }
	SKILLTYPE	Getskilltype() { return skilltype; }

	void	Setangle(int angle) { this->angle = angle; }
	void	Setsize(int size) { this->size = size; }
	void	Setsize2(int size2) { this->size2 = size2; }

	void	ObjectUpdate(TOTALDATA&, int i);
};