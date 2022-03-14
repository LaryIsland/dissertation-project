#pragma once
#include "Component.h"

class MoveComponent : public Component {
public:
	MoveComponent(class Entity* owner, int updateOrder = 10);
	void Update(float deltaTime) override;
	
	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	float GetSideSpeed() const { return mSideSpeed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }
	void SetSideSpeed(float speed) { mSideSpeed = speed; }

private:
	float mAngularSpeed;
	float mForwardSpeed;
	float mSideSpeed;
};
