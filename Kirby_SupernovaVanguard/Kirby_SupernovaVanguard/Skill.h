#pragma once

#include "Object.h"
#include "PlayerData.h"

static int skillID = 0;

enum SKILLTYPE {
	KIRBYSKILL = 1,
	METAKNIGHTSKILL,
	DEDEDESKILL,
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
	SKILLTYPE skilltype;

public:
	Skill() :size(0),skilltype(KIRBYSKILL)
	{}
	~Skill() {}

	
	int	Getsize() { return size; }
	SKILLTYPE	Getskilltype() { return skilltype; }

	void	Setsize(int size) { this->size = size; }
	void	Setskilltype(int i)
	{
		switch (i)
		{
		case KIRBYSKILL:
			this->skilltype = KIRBYSKILL;
			break;
		}
	}

	void	ObjectUpdate(TOTALDATA, int i);
};