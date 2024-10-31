#pragma once
#include "Collider2D.h"

#define BASERADIUS 10

class Circle2D : public Collider2D
{
private:
	int radius;
public:
	Circle2D(int r = BASERADIUS) : Collider2D(CIRCLE), radius(r) {}
	Circle2D(bool isTrigger, ECOLLIDERTYPE type, int radius) : Collider2D(isTrigger, type, CIRCLE), radius(radius) {}
	~Circle2D() {};

	void SetRadius(int r) { radius = r; }

	int GetRadius() { return radius; }

	void DrawCollider(HDC&);
};