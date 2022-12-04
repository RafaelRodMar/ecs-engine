#pragma once

#include "game.h"
#include "AssetsManager.h"
#include "Components.h"
#include "sdl.h"

class SpriteComponent : public Component {
private:
	PositionComponent *position;
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
		position = &entity->getComponent<PositionComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = srcRect.h = 32;
		destRect.w = destRect.h = 64;
	}

	void update() override
	{
		destRect.x = position->x();
		destRect.y = position->y();
	}

	void draw() override
	{
		AssetsManager::Instance()->drawFrame(textureID, position->x(), position->y(), 32, 32, 0, 0, 
			Game::Instance()->getRenderer(), 0.0, 255, SDL_FLIP_NONE);
	}
};

