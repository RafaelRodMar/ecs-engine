#pragma once
#include "components.h"
#include "../Vector2D.h"

class TransformComponent : public Component {
public:
	Vector2D position;

	TransformComponent() {
		position.m_x = 0.0f;
		position.m_y = 0.0f;
	}

	TransformComponent(float x, float y) {
		position = Vector2D(x, y);
	}

	void update() override {
	}

	void setPos(float x, float y) {
		position = Vector2D(x, y);
	}
};