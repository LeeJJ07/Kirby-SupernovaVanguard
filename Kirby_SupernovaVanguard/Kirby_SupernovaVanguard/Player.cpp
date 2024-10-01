#include "Player.h"

void DrawPlayer(HDC hdc, std::vector<Player*> &p)
{
	for (int i = 0; i < p.size(); i++)
	{
		if (!p[i]) continue;
		int left = p[i]->GetPos().x - p[i]->GetRadius();
		int right = p[i]->GetPos().x + p[i]->GetRadius();
		int top = p[i]->GetPos().y - p[i]->GetRadius();
		int bottom = p[i]->GetPos().y + p[i]->GetRadius();
		Ellipse(hdc, left, top, right, bottom);
	}
}