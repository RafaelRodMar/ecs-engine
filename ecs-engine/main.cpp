#include<SDL.h>
#include<SDL_ttf.h>
#include<iostream>
#include<algorithm>
#include<vector>
#include<list>
#include<cmath>
#include "game.h"
#include "json.hpp"
#include "Components.h"
#include "Collision.h"

//map class
class Map* mapa;
//components manager
Manager manager;

//the game class
Game* Game::s_pInstance = 0;

Game::Game() {
	m_pRenderer = NULL;
	m_pWindow = NULL;
}

Game::~Game() {

}

SDL_Window* g_pWindow = 0;
SDL_Renderer* g_pRenderer = 0;

//
std::vector<ColliderComponent*> Game::colliders;

//Entity Component System
auto& player(manager.addEntity()); //creates a new entity
auto& wall(manager.addEntity());

std::string mapfile = "terrain_ss";

//groups
enum groupLabels : std::size_t {
	groupMap,
	groupPlayers,
	groupEnemies,
	groupColliders
};

bool Game::init(const char* title, int xpos, int ypos, int width,
	int height, bool fullscreen)
{
	// store the width and height of the game
	m_gameWidth = width;
	m_gameHeight = height;

	// attempt to initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		int flags = 0;
		if (fullscreen)
		{
			flags = SDL_WINDOW_FULLSCREEN;
		}

		std::cout << "SDL init success\n";
		// init the window
		m_pWindow = SDL_CreateWindow(title, xpos, ypos,
			width, height, flags);
		if (m_pWindow != 0) // window init success
		{
			std::cout << "window creation success\n";
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
			if (m_pRenderer != 0) // renderer init success
			{
				std::cout << "renderer creation success\n";
				SDL_SetRenderDrawColor(m_pRenderer,
					255, 255, 255, 255);
			}
			else
			{
				std::cout << "renderer init fail\n";
				return false; // renderer init fail
			}
		}
		else
		{
			std::cout << "window init fail\n";
			return false; // window init fail
		}
	}
	else
	{
		std::cout << "SDL init fail\n";
		return false; // SDL init fail
	}
	if (TTF_Init() == 0)
	{
		std::cout << "sdl font initialization success\n";
	}
	else
	{
		std::cout << "sdl font init fail\n";
		return false;
	}

	std::cout << "init success\n";
	m_bRunning = true; // everything initiated successfully, start the main loop

	//load images, sounds, music and fonts
	AssetsManager::Instance()->loadAssetsJson();
	Mix_Volume(-1, 16); //adjust sound/music volume for all channels

	//map of the game
	mapa = new Map();
	Map::loadMap("assets/map.map", 25, 20);

	//ecs implementation
	player.addComponent<TransformComponent>(4);
	//player.addComponent<TransformComponent>(10,10); //the player has a position
	player.addComponent<SpriteComponent>("player_anims", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);

	/*wall.addComponent<TransformComponent>(300.0f, 300.0f, 300, 20, 1);
	wall.addComponent<SpriteComponent>("dirt");
	wall.addComponent<ColliderComponent>("wall");
	wall.addGroup(groupMap);*/

	return true;
}

void Game::handleEvents()
{
	InputHandler::Instance()->update();
}

void Game::update()
{
	manager.refresh(); //remove destroyed elements
	manager.update(); //ECS

	for (auto cc : colliders) {
		Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
	}

	/*if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		wall.getComponent<ColliderComponent>().collider))
	{
		player.getComponent<TransformComponent>().scale = 1;
		player.getComponent<TransformComponent>().velocity *= -1;
		std::cout << "wall hit!" << std::endl;
	}*/

	//std::cout << newPlayer.getComponent<PositionComponent>().x() << "," << newPlayer.getComponent<PositionComponent>().y() << std::endl;
	/*player.getComponent<TransformComponent>().position += Vector2D(5, 0);
	if (player.getComponent<TransformComponent>().position.m_x > 100)
	{
		player.getComponent<SpriteComponent>().setTex("enemy");
	}*/
}

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));

void Game::render()
{
	SDL_RenderClear(m_pRenderer);

	for (auto& t : tiles) { t->draw(); }
	for (auto& p : players) { p->draw(); }
	for (auto& e : enemies) { e->draw(); }

	SDL_RenderPresent(m_pRenderer);
}

void Game::AddTile(int srcX, int srcY, int xpos, int ypos) {
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, mapfile);
	tile.addGroup(groupMap);
}

void Game::clean()
{
	AssetsManager::Instance()->clearAllTextures();
	AssetsManager::Instance()->clearSoundsMusic();
	InputHandler::Instance()->clean();
	AssetsManager::Instance()->clearFonts();
	TTF_Quit();
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_Quit();
	m_bRunning = false;
}

void Game::quit()
{
	clean();
}

const int FPS = 60;
const int DELAY_TIME = 1000.0f / FPS;

int main(int argc, char* args[])
{
	Uint32 frameStart, frameTime;

	std::cout << "game init attempt...\n";
	if (Game::Instance()->init("Let's make games", 100, 100, 800, 632,
		false))
	{
		std::cout << "game init success!\n";
		while (Game::Instance()->running())
		{
			frameStart = SDL_GetTicks(); //initial time

			Game::Instance()->handleEvents();
			Game::Instance()->update();
			Game::Instance()->render();

			frameTime = SDL_GetTicks() - frameStart; //final time - initial time

			if (frameTime < DELAY_TIME)
			{
				//con tiempo fijo el retraso es 1000 / 60 = 16,66
				//procesar handleEvents, update y render tarda 1, y hay que esperar 15
				//cout << "frameTime : " << frameTime << "  delay : " << (int)(DELAY_TIME - frameTime) << endl;
				SDL_Delay((int)(DELAY_TIME - frameTime)); //wait until complete the 60 fps
			}
		}
	}
	else
	{
		std::cout << "game init failure - " << SDL_GetError() << "\n";
		return -1;
	}
	std::cout << "game closing...\n";
	Game::Instance()->clean();

	return 0;
}