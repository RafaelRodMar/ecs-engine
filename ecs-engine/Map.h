#pragma once
#include "game.h"

class Map
{
public:
	Map();
	~Map();

	void loadMap(int arr[20][25]);
	void drawMap();

private:
	SDL_Rect src, dest;

	int map[20][25];
};

