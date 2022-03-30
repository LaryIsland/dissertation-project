#pragma once
#include "Entity.h"
#include "BoxComponent.h"
#include "SDL_Scancode.h"

enum PlayerState {
	Standing,
	WalkingRight,
	WalkingLeft,
	Jumping,
	Crouching
};

class PlayerEntity : public Entity {
public:
	PlayerEntity(class Game* game);

	void UpdateEntity(float deltaTime) override;
	void EntityInput(const InputState& state) override;

	PlayerState GetPlayerState() { return currState; }
	void SetPlayerState(PlayerState playerState);

	SDL_Scancode GetRightKey() { return rightKey; }
	SDL_Scancode GetLeftKey() { return leftKey; }
	SDL_Scancode GetUpKey() { return upKey; }
	SDL_Scancode GetDownKey() { return downKey; }

	void SetRightKey(SDL_Scancode key) { rightKey = key; }
	void SetLeftKey(SDL_Scancode key) { leftKey = key; }
	void SetUpKey(SDL_Scancode key) { upKey = key; }
	void SetDownKey(SDL_Scancode key) { downKey = key; }

	void SetControllerNum(int num) { controllerNum = num; }

	BoxComponent* GetBoxComp() { return pBoxComp; }
	void FixCollisions();
private:
	PlayerState currState = Standing;

	class MoveComponent* pMoveComp;
	class BoxComponent* pBoxComp;

	SDL_Scancode rightKey;
	SDL_Scancode leftKey;
	SDL_Scancode upKey;
	SDL_Scancode downKey;

	int controllerNum = 0;

	float sideSpeed;
};