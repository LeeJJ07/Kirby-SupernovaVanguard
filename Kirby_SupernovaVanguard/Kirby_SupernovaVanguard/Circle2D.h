#pragma once
#include "Collider2D.h"

#define BASERADIUS 10

class Circle2D : public Collider2D
{
private:
	int radius;
public:
	Circle2D(POINT p = { 0,0 }, POINT c = {0,0}, short t = 0, int r = BASERADIUS) :Collider2D(p, c, t), radius(r) {}
	~Circle2D() {};
	void SetRadius(int r) { radius = r; }
	int GetRadius() { return radius; }
};