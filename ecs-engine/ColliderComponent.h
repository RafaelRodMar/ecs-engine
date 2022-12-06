#pragma once

#include <string>
#include "sdl.h"
#include "components.h"

class ColliderComponent : public Component {
public:
	SDL_Rect collider;
	std::string tag;

	TransformComponent* transform;

	ColliderComponent(std::string t)
	{
		tag = t;
	}

	void init() override {
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		Game::colliders.push_back(this);
	}

	void update() {
		collider.x = static_cast<int>(transform->position.m_x);
		collider.y = static_cast<int>(transform->position.m_y);
		collider.w = transform->width * transform->scale;
		collider.h = transform->height * transform->scale;
	}
};