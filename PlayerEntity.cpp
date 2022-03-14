#include "PlayerEntity.h"
#include "MoveComponent.h"
#include "SDL_scancode.h"
#include "Renderer.h"
#include "Game.h"

PlayerEntity::PlayerEntity(Game* game) : Entity(game) {
	pMoveComp = new MoveComponent(this);
}

void PlayerEntity::UpdateEntity(float deltaTime) {
	Entity::UpdateEntity(deltaTime);

	Vector3 playerPos = GetPosition();
}

void PlayerEntity::EntityInput(const uint8_t* keys) {
	float forwardSpeed = 0.0f;
	float sideSpeed = 0.0f;
	float angularSpeed = 0.0f;

	if (keys[SDL_SCANCODE_I]) {
		forwardSpeed += 300.0f;
	}
	if (keys[SDL_SCANCODE_K]) {
		forwardSpeed -= 300.0f;
	}
	if (keys[SDL_SCANCODE_J]) {
		sideSpeed -= 300.0f;
	}
	if (keys[SDL_SCANCODE_L]) {
		sideSpeed += 300.0f;
	}
	if (keys[SDL_SCANCODE_U]) {
		angularSpeed -= Math::Pi;
	}
	if (keys[SDL_SCANCODE_O]) {
		angularSpeed += Math::Pi;
	}

	pMoveComp->SetForwardSpeed(forwardSpeed);
	pMoveComp->SetSideSpeed(sideSpeed);
	pMoveComp->SetAngularSpeed(angularSpeed);
}