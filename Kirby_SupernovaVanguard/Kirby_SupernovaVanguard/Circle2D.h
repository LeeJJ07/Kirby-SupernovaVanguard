#pragma once
#include "Collider2D.h"

#define BASERADIUS 20

class Circle2D : public Collider2D
{
private:
	int radius;
public:
	Circle2D(int r = BASERADIUS) : Collider2D(CIRCLE), radius(r) {}
	Circle2D(bool isTrigger, ColliderType type) : Collider2D(isTrigger, type, CIRCLE), radius(BASERADIUS) {}
	~Circle2D() {};

	void SetRadius(int r) { radius = r; }

	int GetRadius() { return radius; }

	void DrawCollider(HDC&, int& ID);
};