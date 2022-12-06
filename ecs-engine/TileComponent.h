#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "sdl.h"

class TileComponent : public Component {
public:
	TransformComponent *transform;
	SpriteComponent *sprite;

	SDL_Rect tileRect;
	int tileID;
	//const char* path;
	std::string textureID;

	TileComponent() = default;

	TileComponent(int x, int y, int w, int h, int id) {
		tileRect.x = x;
		tileRect.y = y;
		tileRect.w = w;
		tileRect.h = h;
		tileID = id;

		switch (tileID)
		{
		case 0:
			textureID = "dirt";
			break;
		case 1:
			textureID = "grass";
			break;
		case 2:
			textureID = "water";
			break;
		default:
			break;
		}
	}

	void init() override {
		entity->addComponent<TransformComponent>((float)tileRect.x, (float)tileRect.y, tileRect.w, tileRect.h, 1);
		transform = &entity->getComponent<TransformComponent>();

		entity->addComponent<SpriteComponent>(textureID);
		sprite = &entity->getComponent<SpriteComponent>();
	}
};