#pragma once

#include "game.h"
#include "ECS.h"
#include "Components.h"
#include "SDL.h"
#include "InputHandler.h"

class KeyboardController : public Component {
public:
	TransformComponent *transform;
	SpriteComponent *sprite;

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override {
		transform->velocity = Vector2D(0, 0);
		sprite->Play("Idle");
		if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_W))
		{
			transform->velocity.m_y = -1;
			sprite->Play("Walk");
		}
		if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_A))
		{
			transform->velocity.m_x = -1;
			sprite->Play("Walk");
			sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
		}
		if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_D))
		{
			transform->velocity.m_x = 1;
			sprite->Play("Walk");
			sprite->spriteFlip = SDL_FLIP_NONE;
		}
		if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_S))
		{
			transform->velocity.m_y = 1;
			sprite->Play("Walk");
		}
		if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
		{
			Game::m_bRunning = false;
		}
	}
};