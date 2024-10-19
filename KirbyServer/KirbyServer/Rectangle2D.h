#pragma once
#include "Collider2D.h"

#define BASESIZE 10

class Rectangle2D : public Collider2D
{
private:
	int size;
public:
	Rectangle2D(int size = BASESIZE) : Collider2D(), size(size) {}
	Rectangle2D(bool isTrigger, ColliderType type) : Collider2D(isTrigger, type), size(BASESIZE) {}
	~Rectangle2D() {};

	void SetRadius(int r) { size = r; }

	int GetRadius() { return size; }

	void DrawCollider(HDC&);
};