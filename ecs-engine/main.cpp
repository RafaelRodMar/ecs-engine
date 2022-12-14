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
#include <sstream>

//map class
class Map* mapa;
//components manager
Manager manager;

//the game class
Game* Game::s_pInstance = 0;

//camera
SDL_Rect Game::camera = { 0,0,800,640 };

Game::Game() {
	m_pRenderer = NULL;
	m_pWindow = NULL;
}

Game::~Game() {

}

SDL_Window* g_pWindow = 0;
SDL_Renderer* g_pRenderer = 0;

//
bool Game::m_bRunning = false;

//Entity Component System
auto& player(manager.addEntity()); //creates a new entity
auto& label(manager.addEntity());

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
	mapa = new Map("terrain_ss", 3, 32);
	mapa->loadMap("assets/map.map", 25, 20);

	//ecs implementation
	player.addComponent<TransformComponent>(400, 640, 32, 32, 4);
	//player.addComponent<TransformComponent>(4);
	//player.addComponent<TransformComponent>(10,10); //the player has a position
	player.addComponent<SpriteComponent>("player_anims", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);

	SDL_Color white = { 255,255,255,255 };
	label.addComponent<UILabel>(10, 10, "Test string", "font", white);

	CreateProjectile(Vector2D(600, 600), Vector2D(2,0), 200, 2, "proj_test");
	CreateProjectile(Vector2D(600, 620), Vector2D(2, 0), 200, 2, "proj_test");
	CreateProjectile(Vector2D(400, 600), Vector2D(2, 1), 200, 2, "proj_test");
	CreateProjectile(Vector2D(600, 600), Vector2D(2, -1), 200, 2, "proj_test");

	/*wall.addComponent<TransformComponent>(300.0f, 300.0f, 300, 20, 1);
	wall.addComponent<SpriteComponent>("dirt");
	wall.addComponent<ColliderComponent>("wall");
	wall.addGroup(groupMap);*/

	return true;
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));

void Game::handleEvents()
{
	InputHandler::Instance()->update();
}

void Game::update()
{
	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	std::stringstream ss;
	ss << "Player position: " << playerPos.m_x << "," << playerPos.m_y;
	label.getComponent<UILabel>().SetLabelText(ss.str(), "font");

	manager.refresh(); //remove destroyed elements
	manager.update(); //ECS

	for (auto& c : colliders) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol))
		{
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	for (auto& p : projectiles) {
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
		{
			p->destroy();
		}
	}

	//camera
	camera.x = player.getComponent<TransformComponent>().position.m_x - 400;
	camera.y = player.getComponent<TransformComponent>().position.m_y - 320;

	if (camera.x < 0) camera.x = 0;
	if (camera.y < 0) camera.y = 0;
	if (camera.x > camera.w) camera.x = camera.w;
	if (camera.y > camera.h) camera.y = camera.h;

	//scrolling
	//Vector2D pVel = player.getComponent<TransformComponent>().velocity;
	//int pSpeed = player.getComponent<TransformComponent>().speed;

	////make tiles move at the same speed than player.
	//for (auto t : tiles) {
	//	t->getComponent<TileComponent>().destRect.x += -(pVel.m_x * pSpeed);
	//	t->getComponent<TileComponent>().destRect.y += -(pVel.m_y * pSpeed);
	//}
	//end scrolling

	/*for (auto cc : colliders) {
		Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
	}*/

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

void Game::render()
{
	SDL_RenderClear(m_pRenderer);

	for (auto& t : tiles) { t->draw(); }
	//for (auto& c : colliders) { c->draw(); } //visible colliders.
	for (auto& p : players) { p->draw(); }
	for (auto& p : projectiles) { p->draw(); }
	label.draw();

	SDL_RenderPresent(m_pRenderer);
}

void Game::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id) {
	auto& projectile(manager.addEntity());
	projectile.addComponent<TransformComponent>(pos.m_x, pos.m_y, 32, 32, 1);
	projectile.addComponent<SpriteComponent>(id, false);
	projectile.addComponent<ProjectileComponent>(range, speed, vel);
	projectile.addComponent<ColliderComponent>("projectile");
	projectile.addGroup(Game::groupProjectiles);
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
	if (Game::Instance()->init("Let's make games", 100, 100, 800, 640,
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