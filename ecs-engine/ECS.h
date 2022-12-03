#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;

using ComponentID = std::size_t;

//get a ID for the next component.
inline ComponentID getComponentTypeID() {
	static ComponentID lastID = 0;
	return lastID++;
}

//get a ID for the next component, called by component type.
template <typename T> inline ComponentID getComponentTypeID() noexcept {
	static ComponentID typeID = getComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;

using ComponentBitSet = std::bitset<maxComponents>; //for checking if a entity have any components.
using ComponentArray = std::array<Component*, maxComponents>; //array with components pointers.

class Component {
public:
	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~Component() {}
};

class Entity {
private:
	bool active = true; //if it is false, then it should be destroyed.
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;

public:
	void update() {
		for (auto& c : components) c->update();
		for (auto& c : components) c->draw();
	}

	void draw() {}
	bool isActive() {
		return active;
	}
	void destroy() { active = false; }

	template <typename T> bool hasComponent() const {
		return componentBitSet[getComponentTypeID<T>()];
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs) {
		T* c(new T(std::forward<TArgs>(mArgs)...)); //create new component.
		c->entity = this;						    //set the entity as owner of the component.
		std::unique_ptr<Component> uPtr{ c };       //wrap the pointer with a unique_ptr.
		components.emplace_back(std::move(uPtr));   //add component to the vector.

		componentArray[getComponentTypeID<T>()] = c;  //add pointer to the array of components.
		componentBitSet[getComponentTypeID<T>()] = true; //set the bit in the bitset.

		c->init();  //initialize the component.
		return *c;
	}

	template<typename T> T& getComponent() const {
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}
};

class Manager
{
private:
	std::vector<std::unique_ptr<Entity>> entities;

public:
	void update() {
		for (auto& e : entities) e->update();
	}
	void draw() {
		for (auto& e : entities) e->draw();
	}

	void refresh() {
		entities.erase(std::remove_if(std::begin(entities), std::end(entities), [](const std::unique_ptr<Entity> &mEntity) {
			return !mEntity->isActive();
		}), std::end(entities));
	}

	Entity& addEntity() {
		Entity* e = new Entity();
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
};

