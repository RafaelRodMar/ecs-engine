#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>

class AssetManager;
class ColliderComponent;

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	void render();

	bool running() { return isRunning; }
	void clean();

	static SDL_Renderer *renderer;
	static bool isRunning;

private:

	int cnt = 0;
	SDL_Window *window;
};
