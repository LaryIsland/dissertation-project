#include "PlayerEntity.h"
#include "MoveComponent.h"
#include "SDL_scancode.h"
#include "Renderer.h"
#include "Game.h"
#include "InputSystem.h"

PlayerEntity::PlayerEntity(Game* game) : Entity(game) {
	pMoveComp = new MoveComponent(this);
}

void PlayerEntity::UpdateEntity(float deltaTime) {
	Entity::UpdateEntity(deltaTime);
}

void PlayerEntity::EntityInput(const InputState& state) {
	float forwardSpeed = 0.0f;
	float sideSpeed = 0.0f;
	float angularSpeed = 0.0f;

	if (state.Keyboard.GetKeyState(rightKey) == RisingEdge || state.Keyboard.GetKeyState(rightKey) == Holding) {
		forwardSpeed += 300.0f;
	}
	if (state.Keyboard.GetKeyState(leftKey) == RisingEdge || state.Keyboard.GetKeyState(leftKey) == Holding) {
		forwardSpeed -= 300.0f;
	}
	if (state.Keyboard.GetKeyState(upKey) == RisingEdge && GetPosition().y <= 0.0f) {
		//verticalSpeed += 300.0f;
	}
	if (state.Keyboard.GetKeyState(downKey) == RisingEdge || state.Keyboard.GetKeyState(downKey) == Holding && GetPosition().y <= 0.0f) {
		//Crouch
	}

	pMoveComp->SetForwardSpeed(forwardSpeed);
	pMoveComp->SetSideSpeed(sideSpeed);
	pMoveComp->SetAngularSpeed(angularSpeed);
}