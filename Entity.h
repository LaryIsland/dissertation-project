#pragma once
#include <vector>
#include "Math.h"
#include <cstdint>
#include "InputSystem.h"

class Entity {
public:
	enum State {
		Alive,
		Dead
	};

	Entity(class Game* game);
	virtual ~Entity();

	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateEntity(float deltaTime);

	void ProcessInput(const InputState& state);
	virtual void EntityInput(const InputState& state);

	const Vector3& GetPosition() const { return ePosition; }
	void SetPosition(const Vector3& pos) { ePosition = pos; eRecalculateWorldTransform = true; }
	float GetScale() const { return eScale; }
	void SetScale(float scale) { eScale = scale; eRecalculateWorldTransform = true; }
	const Quaternion& GetRotation() const { return eRotation; }
	void SetRotation(const Quaternion& rotation) { eRotation = rotation; eRecalculateWorldTransform = true; }

	void CalculateWorldTransform();
	const Matrix4& GetWorldTransform() const { return eWorldTransform; }

	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, eRotation); }
	Vector3 GetSideward() const { return Vector3::Transform(Vector3::UnitY, eRotation); }

	State GetState() const { return eState; }
	void SetState(State state) { eState = state; }

	class Game* GetGame() { return gGame; }

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	State eState;

	Matrix4 eWorldTransform;
	Vector3 ePosition;
	Quaternion eRotation;
	float eScale;
	bool eRecalculateWorldTransform;

	std::vector<class Component*> eComponents;
	class Game* gGame;
};
