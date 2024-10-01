#pragma once
#include "Collider2D.h"

#define BASERADIUS 10

class Circle2D : public Collider2D
{
private:
	int radius;
public:
	Circle2D(int x = 0, int y = 0, int r = BASERADIUS) :Collider2D(x, y), radius(r) {}
	~Circle2D() {};
	void SetRadius(int r) { radius = r; }
	int GetRadius() { return radius; }
};