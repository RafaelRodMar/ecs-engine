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

	bool animated = false;
	int frames = 0;
	int cFrame = 0; //current frame.
	int speed = 100;

public:
	SpriteComponent() = default;
	SpriteComponent(const string id) {
		setTex(id);
	}

	SpriteComponent(const string id, int nFrames, int mSpeed) {
		setTex(id);
		animated = true;
		frames = nFrames;
		speed = mSpeed;
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
		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
			cFrame = (SDL_GetTicks() / speed) % frames; //only useful line in update. The rest is from BirchEngine
		}

		destRect.x = static_cast<int>(transform->position.m_x);
		destRect.y = static_cast<int>(transform->position.m_y);
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void draw() override
	{
		AssetsManager::Instance()->drawFrame(textureID, (int)transform->position.m_x, (int)transform->position.m_y, 32, 32, 0, cFrame, 
			Game::Instance()->getRenderer(), 0.0, 255, SDL_FLIP_NONE);
	}
};

