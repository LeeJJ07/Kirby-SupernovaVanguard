#pragma once
#include "DededeSkill.h"
#include "EletricfieldSkill.h"
#include "KirbySkill.h"
#include "KunaiSkill.h"
#include "MaberoaSkill.h"
#include "MagicarrowSkill.h"
#include "MetaknightSkill.h"
#include "TornadoSkill.h"
#include "TruckSkill.h"

static void DrawSkill(HDC hdc, Skill* object)
{
	switch (object->Getskilltype())
	{
	case KIRBYSKILL:
		break;
	case DEDEDESKILL:
		break;
	case METAKNIGHTSKILL:
		break;
	case MABEROASKILL:
		break;
	case ELECTRICFIELDSKILL:
		break;
	case KUNAISKILL:
		break;
	case MAGICARROWSKILL:
		break;
	case TORNADOSKILL:
		break;
	case TRUCKSKILL:
		break;
	}
}