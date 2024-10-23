#include "Rectangle2D.h"

void Rectangle2D::DrawCollider(HDC& hdc, int& ID)
{
	POINT point[4];
	point[0] = { - width , - height };
	point[1] = { + width , - height };
	point[2] = { + width , + height };
	point[3] = { - width , + height };

	int rectangleLine = round((float)sqrt(pow(width, 2) + pow(height, 2)) / 2);

	float degree[2];
	degree[1] = atan((float)height / width) * 2 * 180 / 3.1415926;
	degree[0] = 180 - degree[1];
	 
	angle += degree[1] / 2;

	float angleOffset = 0;

	for (int i = 0; i < 4; i++)
	{
		angleOffset += degree[i % 2];
		point[i].x = rectangleLine * cos((angle + angleOffset) / 180.0 * 3.1415926) + GetPosition().x;
		point[i].y = rectangleLine * sin((angle + angleOffset) / 180.0 * 3.1415926) + GetPosition().y;
	}

	Polygon(hdc, point, 4);

	CString t;

	t.Format(_T("%d"), ID);

	SetTextColor(hdc, RGB(0, 0, 0));

	TextOut(hdc, GetPosition().x, GetPosition().y, t, t.GetLength());
}