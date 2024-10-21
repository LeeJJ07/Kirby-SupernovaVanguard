#pragma once

#define MAX_MAP_SIZE_X 4000
#define MAX_MAP_SIZE_Y 2000

class Collider2D;

void InitMap(Collider2D*** &);

template<class T>
void Create(T* t, Collider2D***& _map)
{
	_map[t->GetPos().y][t->GetPos().x] = new T();
	_map[t->GetPos().y][t->GetPos().x] = t;
}