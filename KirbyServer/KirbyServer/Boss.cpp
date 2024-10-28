#include "Boss.h"
//#include "FireballSkill.h"
//#include "LaserSkill.h"

int BossID;

void Boss::StateUpdate()
{
	switch (curState)
	{
	case CHASE:
		break;
	case ATTACK:
		break;
	case DEATH:
		break;
	}
}

void Boss::Update()
{
	lookingDirection = NormalizationDir(targetPos);

	position.x += (int)round(lookingDirection.first * speed);
	position.y += (int)round(lookingDirection.second * speed);

	Settime_2();

	double phaseChangeTime = std::chrono::duration_cast<std::chrono::duration<double>>(Gettime_2() - Gettime_1()).count();

	if (phaseChangeTime >= PHASECHANGETIME)
	{
		phase++;
		phase %= PHASENUM;
		std::vector<SkillManager*> sm = this->GetSkillManager();
		sm.clear();
		this->SetSkillManager(sm);

		Monster* monster = this;

		switch(phase)
		{
		case Electric:
		{
			InitLasorSkill(monster);
		}
			break;
		case Flame:
		{
			InitFireballSkill(monster);
		}
			break;
		}

		Settime_1();
	}
}