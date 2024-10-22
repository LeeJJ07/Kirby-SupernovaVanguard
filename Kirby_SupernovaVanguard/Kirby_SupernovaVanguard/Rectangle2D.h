#pragma once
#include "Collider2D.h"

#define BASESIZE 10

class Rectangle2D : public Collider2D
{
private:
	int width;
	int height;
	int angle;
public:
	Rectangle2D(int width = 0, int height = 0) : Collider2D(), width(width), height(height) {}
	Rectangle2D(bool isTrigger, ColliderType type) : Collider2D(isTrigger, type), width(BASESIZE), height(BASESIZE) {}
	~Rectangle2D() {};

	int GetWidth() { return width; }
	int GetHeight() { return height; }
	int Getangle() { return angle; }

	void SetWidth(int width) { this->width = width; }
	void SetHeight(int height) { this->height = height; }
	void Setangle(int angle) { this->angle = angle; }

	void DrawCollider(HDC&, int&);
};