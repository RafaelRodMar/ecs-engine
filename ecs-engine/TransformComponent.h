#pragma once
#include "components.h"
#include "Vector2D.h"

class TransformComponent : public Component {
public:
	Vector2D position;
	Vector2D velocity;

	int speed = 3;

	TransformComponent() {
		position = Vector2D(0.0f,0.0f);
	}

	TransformComponent(float x, float y) {
		position = Vector2D(x, y);
	}

	void init() override {
		velocity = Vector2D(0, 0);
	}

	void update() override {
		position += velocity * speed;
	}

	void setPos(float x, float y) {
		position = Vector2D(x, y);
	}
};