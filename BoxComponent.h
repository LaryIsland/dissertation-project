#pragma once
#include "Component.h"
#include "Collision.h"

class BoxComponent : public Component {
public:
	BoxComponent(class Entity* owner, int updateOrder = 100);
	~BoxComponent();

	void SetObjectBox(const AABB& model) { objectBox = model; }
	const AABB& GetWorldBox() const { return worldBox; }

	void SetShouldRotate(bool value) { shouldRotate = value; }
private:
	AABB objectBox;
	AABB worldBox;
	bool shouldRotate;
};