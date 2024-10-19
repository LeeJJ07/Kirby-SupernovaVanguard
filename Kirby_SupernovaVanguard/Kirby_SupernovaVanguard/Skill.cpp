#include "Skill.h"

void Skill::ObjectUpdate(TOTALDATA totalData, int i)
{
	
	switch (totalData.sdata[i].collidertype)
	{
	case CIRCLEFIGURE:
		Circle2D* skillcirclecollider;
		skillcirclecollider = dynamic_cast<Circle2D*>(this->GetCollider());
		skillcirclecollider->SetRadius(totalData.sdata[i].collidersize);
		break;
	case RECTANGLEFIGURE:
		Rectangle2D* skillrectanglecollider;
		skillrectanglecollider = dynamic_cast<Rectangle2D*>(this->GetCollider());
		skillrectanglecollider->SetSize(totalData.sdata[i].collidersize);
		break;
	}
	SetPosition(totalData.sdata[i].position);
	GetCollider()->MovePosition(totalData.sdata[i].colliderposition);
	Setsize(totalData.sdata[i].size);
	Setskilltype(totalData.sdata[i].skilltype);
}