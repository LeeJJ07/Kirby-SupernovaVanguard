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
		skillrectanglecollider->SetWidth(totalData.sdata[i].collidersize);
		skillrectanglecollider->SetHeight(totalData.sdata[i].collidersize2);
		break;
	}
	SetPosition(totalData.sdata[i].position);
	GetCollider()->MovePosition(totalData.sdata[i].colliderposition);
	Setsize(totalData.sdata[i].size);
	Setsize2(totalData.sdata[i].size2);
}