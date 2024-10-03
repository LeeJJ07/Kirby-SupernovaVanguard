#pragma once

#define MAX_MAP_SIZE_X 5000
#define MAX_MAP_SIZE_Y 3000

class Collider2D;

void InitMap(Collider2D*** &);

template<class T>
void Create(T* t, Collider2D***& map)
{
	map[t->GetPos().y][t->GetPos().x] = new T();
	map[t->GetPos().y][t->GetPos().x] = t;
}