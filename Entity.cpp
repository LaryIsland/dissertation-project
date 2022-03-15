#include "Entity.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Entity::Entity(Game* game) :
	eState(Alive),
	ePosition(Vector3::Zero),
	eRotation(Quaternion::Identity),
	eScale(1.0f),
	gGame(game),
	eRecalculateWorldTransform(true) {
	gGame->AddEntity(this);
}

Entity::~Entity() {
	gGame->RemoveEntity(this);
	while (!eComponents.empty()) {
		delete eComponents.back();
	}
}

void Entity::Update(float deltaTime) {
	if (eState == Alive) {
		CalculateWorldTransform();

		UpdateComponents(deltaTime);
		UpdateEntity(deltaTime);

		CalculateWorldTransform();
	}
}

void Entity::UpdateComponents(float deltaTime) {
	for (auto comp : eComponents) {
		comp->Update(deltaTime);
	}
}

void Entity::UpdateEntity(float deltaTime) {}

void Entity::ProcessInput(const InputState& state) {
	if (eState == Alive) {
		for (auto comp : eComponents) {
			comp->ProcessInput(state);
		}

		EntityInput(state);
	}
}

void Entity::EntityInput(const InputState& input) {}

void Entity::CalculateWorldTransform() {
	if (eRecalculateWorldTransform) {
		eRecalculateWorldTransform = false;
		eWorldTransform = Matrix4::CreateScale(eScale);
		eWorldTransform *= Matrix4::CreateFromQuaternion(eRotation);
		eWorldTransform *= Matrix4::CreateTranslation(ePosition);

		for (auto comp : eComponents) {
			comp->OnUpdateWorldTransform();
		}
	}
}

void Entity::AddComponent(Component* component) {
	int myOrder = component->GetUpdateOrder();
	auto iter = eComponents.begin();
	for (; iter != eComponents.end(); ++iter) {
		if (myOrder < (*iter)->GetUpdateOrder()) {
			break;
		}
	}

	eComponents.insert(iter, component);
}

void Entity::RemoveComponent(Component* component) {
	auto iter = std::find(eComponents.begin(), eComponents.end(), component);
	if (iter != eComponents.end()) {
		eComponents.erase(iter);
	}
}
