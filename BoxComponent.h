#pragma once
#include "Component.h"
#include "Collision.h"

class BoxComponent : public Component {
public:
	BoxComponent(class Entity* owner, int updateOrder = 100);
	~BoxComponent();

	void SetObjectBox(const AABB& model) { mObjectBox = model; }
	const AABB& GetWorldBox() const { return mWorldBox; }

	void SetShouldRotate(bool value) { mShouldRotate = value; }
private:
	AABB mObjectBox;
	AABB mWorldBox;
	bool mShouldRotate;
};