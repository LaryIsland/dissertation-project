#include "Math.h"
#include "MoveComponent.h"
#include "Entity.h"

MoveComponent::MoveComponent(class Entity* owner, int updateOrder) :
	Component(owner, updateOrder),
	mAngularSpeed(0.0f),
	mForwardSpeed(0.0f),
	mSideSpeed(0.0f)
{}

void MoveComponent::Update(float deltaTime) {
	if (!Math::NearZero(mAngularSpeed)) {
		Quaternion rot = cOwner->GetRotation();
		float angle = mAngularSpeed * deltaTime;
		Quaternion inc(Vector3::UnitZ, angle);
		rot = Quaternion::Concatenate(rot, inc);
		cOwner->SetRotation(rot);
	}

	if (!Math::NearZero(mForwardSpeed)) {
		Vector3 pos = cOwner->GetPosition();
		pos += cOwner->GetForward() * mForwardSpeed * deltaTime;
		cOwner->SetPosition(pos);
	}

	if (!Math::NearZero(mSideSpeed)) {
		Vector3 pos = cOwner->GetPosition();
		pos += Vector3::UnitX * mSideSpeed * deltaTime;
		cOwner->SetPosition(pos);
	}
}