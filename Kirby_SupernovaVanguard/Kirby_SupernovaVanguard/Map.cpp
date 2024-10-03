#include "Map.h"

void InitMap(Collider2D*** &map)
{
	map = new Collider2D**[MAX_MAP_SIZE_Y];

	for (int i = 0; i < MAX_MAP_SIZE_Y; i++)
	{
		map[i] = new Collider2D*[MAX_MAP_SIZE_X];
	}
}