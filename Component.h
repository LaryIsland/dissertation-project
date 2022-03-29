#pragma once
#include <cstdint>
#include "InputSystem.h"

class Component {
public:
	Component(class Entity* owner, int updateOrder = 100);
	virtual ~Component();
	virtual void Update(float deltaTime);
	virtual void ProcessInput(const InputState& input) {}
	virtual void OnUpdateWorldTransform() {}

	class Entity* GetOwner() { return cOwner; }
	int GetUpdateOrder() const { return cUpdateOrder; }

protected:
	class Entity* cOwner;
	int cUpdateOrder;
};