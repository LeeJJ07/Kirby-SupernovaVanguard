#pragma once
#include "Collider2D.h"

#define BASESIZE 10

class Rectangle2D : public Collider2D
{
private:
	int width;
	int height;
public:
	Rectangle2D(int width = 0, int height = 0) : Collider2D(), width(width), height(height) {}
	Rectangle2D(bool isTrigger, ColliderType type) : Collider2D(isTrigger, type), width(0),height(0) {}
	~Rectangle2D() {};

	int GetWidth() { return width; }
	int GetHeight() { return height; }

	void SetWidth(int width) { this->width = width; }
	void SetHeight(int height) { this->height = height; }

	void DrawCollider(HDC&);
};