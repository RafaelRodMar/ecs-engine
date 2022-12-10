#pragma once

#include "game.h"
#include "AssetsManager.h"
#include "Components.h"
#include "sdl.h"
#include "Animation.h"
#include <map>

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

	int animIndex = 0;
	std::map<const char*, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(const string id) {
		setTex(id);
	}

	SpriteComponent(const string id, bool isAnimated) {
		setTex(id);
		animated = isAnimated;

		Animation idle = Animation(0, 3, 100);
		Animation walk = Animation(1, 8, 100);

		animations.emplace("Idle", idle);
		animations.emplace("Walk", walk);

		Play("Idle");
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
			cFrame = (SDL_GetTicks() / speed) % frames;
		}

		srcRect.y = animIndex * transform->height; //for sprite sheets with various animations.

		destRect.x = static_cast<int>(transform->position.m_x) - Game::camera.x;
		destRect.y = static_cast<int>(transform->position.m_y) - Game::camera.y;
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void draw() override
	{
		AssetsManager::Instance()->drawFrameScl(textureID, destRect.x, destRect.y, srcRect.w, srcRect.h, destRect.w, destRect.h, animIndex, cFrame,
			Game::Instance()->getRenderer(), 0.0, 255, spriteFlip);
	}

	void Play(const char* animName) {
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	}
};

