#include "PlayerEntity.h"
#include "MoveComponent.h"
#include "SDL_scancode.h"
#include "Renderer.h"
#include "Game.h"
#include "InputSystem.h"
#include <iostream>
using namespace std;

PlayerEntity::PlayerEntity(Game* game) : Entity(game) {
	pMoveComp = new MoveComponent(this);
	rightKey = SDL_SCANCODE_D;
	leftKey = SDL_SCANCODE_A;
	upKey = SDL_SCANCODE_W;
	downKey = SDL_SCANCODE_S;
	sideSpeed = 300.0f;
}

void PlayerEntity::UpdateEntity(float deltaTime) {
	Entity::UpdateEntity(deltaTime);
}

void PlayerEntity::EntityInput(const InputState& state) {
	switch (state.Keyboard.GetKeyState(rightKey)) {
	case RisingEdge:
	case Holding:
		SetPlayerState(WalkingRight);
		break;
	case FallingEdge:
		SetPlayerState(Standing);
		break;
	}

	switch (state.Keyboard.GetKeyState(leftKey)) {
	case RisingEdge:
	case Holding:
		SetPlayerState(WalkingLeft);
		break;
	case FallingEdge:
		SetPlayerState(Standing);
		break;
	}

	switch (state.Keyboard.GetKeyState(upKey)) {
	case RisingEdge:
		SetPlayerState(Jumping);
		break;
	}

	switch (state.Keyboard.GetKeyState(downKey)) {
	case RisingEdge:
	case Holding:
		SetPlayerState(Crouching);
		break;
	}
}

void PlayerEntity::SetPlayerState(PlayerState playerState) {
	switch (playerState) {
	case Standing:
		if (currState == WalkingLeft || currState == WalkingRight) {
			currState = Standing;
			cout << "Standing\n";
			pMoveComp->SetSideSpeed(0);

		}
		break;
	case WalkingRight:
		if (currState == Standing) {
			currState = WalkingRight;
			cout << "Walking Right\n";
			pMoveComp->SetSideSpeed(sideSpeed);
		}
		break;
	case WalkingLeft:
		if (currState == Standing) {
			currState = WalkingLeft;
			cout << "Walking Left\n";
			pMoveComp->SetSideSpeed(-sideSpeed);
		}
		break;
	}
}