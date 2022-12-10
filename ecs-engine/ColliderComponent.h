#pragma once

#include <string>
#include "sdl.h"
#include "components.h"
#include "AssetsManager.h"

class ColliderComponent : public Component {
public:
	SDL_Rect collider;
	std::string tag;

	std::string textureID;
	SDL_Rect srcR, destR;

	TransformComponent* transform;

	ColliderComponent(std::string t)
	{
		tag = t;
	}

	ColliderComponent(std::string t, int xpos, int ypos, int size) {
		tag = t;
		collider.x = xpos;
		collider.y = ypos;
		collider.h = collider.w = size;
	}

	void init() override {
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		textureID = "coltex";
		srcR = { 0,0,32,32 };
		destR = {collider.x, collider.y, collider.w, collider.h};
	}

	void update() {
		if (tag != "terrain")
		{
			collider.x = static_cast<int>(transform->position.m_x);
			collider.y = static_cast<int>(transform->position.m_y);
			collider.w = transform->width * transform->scale;
			collider.h = transform->height * transform->scale;
		}

		destR.x = collider.x - Game::camera.x;
		destR.y = collider.y - Game::camera.y;
	}

	void draw() override {
		AssetsManager::Instance()->drawFrameScl(textureID, destR.x, destR.y, srcR.w, srcR.h, destR.w, destR.h, 0, 0, Game::Instance()->getRenderer(), 0.0, 255, SDL_FLIP_NONE);
	}
};