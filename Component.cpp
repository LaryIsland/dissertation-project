#include "Component.h"
#include "Entity.h"

Component::Component(Entity* owner, int updateOrder):
	cOwner(owner),
	cUpdateOrder(updateOrder) {
	cOwner->AddComponent(this);
}

Component::~Component() { cOwner->RemoveComponent(this); }

void Component::Update(float deltaTime) {}