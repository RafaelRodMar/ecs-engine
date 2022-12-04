#pragma once

#include "game.h"
#include "ECS.h"
#include "Components.h"
#include "SDL.h"
#include "InputHandler.h"

class KeyboardController : public Component {
public:
	TransformComponent *transform;

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override {
		transform->velocity = Vector2D(0, 0);
		if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_W)) transform->velocity.m_y = -1;
		if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_A)) transform->velocity.m_x = -1;
		if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_D)) transform->velocity.m_x = 1;
		if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_S)) transform->velocity.m_y = 1;
	}
};