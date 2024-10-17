#include "Skill.h"

Circle2D* skillcirclecollider;

void Skill::ObjectUpdate(TOTALDATA totalData, int i)
{
	
	switch (totalData.sdata[i].collidertype)
	{
	case CIRCLEFIGURE:
		skillcirclecollider = dynamic_cast<Circle2D*>(this->GetCollider());
		skillcirclecollider->SetRadius(totalData.sdata[i].collidersize);
		break;
	}
	SetPosition(totalData.sdata[i].position);
	GetCollider()->MovePosition(totalData.sdata[i].colliderposition);
	Setsize(totalData.sdata[i].size);
	Setskilltype(totalData.sdata[i].skilltype);
}