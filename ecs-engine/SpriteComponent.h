#pragma once

#include "game.h"
#include "AssetsManager.h"
#include "Components.h"
#include "sdl.h"

class SpriteComponent : public Component {
private:
	TransformComponent *transform;
	std::string textureID;
	SDL_Rect srcRect, destRect;

public:
	SpriteComponent() = default;
	SpriteComponent(const string id) {
		setTex(id);
	}

	void setTex(const string id) {
		textureID = id;
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = srcRect.h = 32;
		destRect.w = destRect.h = 64;
	}

	void update() override
	{
		destRect.x = (int)transform->position.m_x;
		destRect.y = (int)transform->position.m_y;
	}

	void draw() override
	{
		AssetsManager::Instance()->drawFrame(textureID, (int)transform->position.m_x, (int)transform->position.m_y, 32, 32, 0, 0, 
			Game::Instance()->getRenderer(), 0.0, 255, SDL_FLIP_NONE);
	}
};

