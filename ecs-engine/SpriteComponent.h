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
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void update() override
	{
		destRect.x = static_cast<int>(transform->position.m_x);
		destRect.y = static_cast<int>(transform->position.m_y);
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void draw() override
	{
		AssetsManager::Instance()->drawFrame(textureID, (int)transform->position.m_x, (int)transform->position.m_y, 32, 32, 0, 0, 
			Game::Instance()->getRenderer(), 0.0, 255, SDL_FLIP_NONE);
	}
};

