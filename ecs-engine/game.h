#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>
#include <list>
#include "Vector2D.h"
#include "AssetsManager.h"
#include "InputHandler.h"
#include "Map.h"
#include "ECS.h"

class ColliderComponent;

class Game
{
public:
	static Game* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new Game();
			return s_pInstance;
		}
		return s_pInstance;
	}
	~Game();

	SDL_Renderer* getRenderer() const { return m_pRenderer; }

	bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	void render();

	bool running() { return m_bRunning; }
	void clean();
	void quit();

	int getGameWidth() const { return m_gameWidth; }
	int getGameHeight() const { return m_gameHeight; }

	static void AddTile(int srcX, int srcY, int xpos, int ypos);
	static std::vector<ColliderComponent*> colliders;
	static bool m_bRunning;

private:
	Game();
	static Game* s_pInstance;
	SDL_Renderer* m_pRenderer;
	SDL_Window *m_pWindow;

	int m_gameWidth;
	int m_gameHeight;
};
