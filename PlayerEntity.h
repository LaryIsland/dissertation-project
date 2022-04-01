#pragma once
#include "Entity.h"
#include "BoxComponent.h"
#include "SDL_Scancode.h"

enum PlayerState {
	Standing,
	WalkingRight,
	WalkingLeft,
	Punching,
	HitStun,
	Victory,
	Defeat
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
	SDL_Scancode GetUpKey() { return punchKey; }

	int GetHealth() { return pHealth; }

	void ReduceHealth() {
		pHealth -= 1;
		if (pHealth == 0) {
			SetPlayerState(Defeat);
			pEnemy->SetPlayerState(Victory);
		}
	}

	void SetRightKey(SDL_Scancode key) { rightKey = key; }
	void SetLeftKey(SDL_Scancode key) { leftKey = key; }
	void SetPunchKey(SDL_Scancode key) { punchKey = key; }

	void SetControllerNum(int num) { controllerNum = num; }

	BoxComponent* GetBoxComp() { return pBoxComp; }
	void FixCollisions();

	bool AnimationLock(float deltaTime);

	void SetEnemyPlayer(PlayerEntity* enemy) { pEnemy = enemy; }

private:
	PlayerState currState;

	class PlayerEntity* pEnemy;

	class MoveComponent* pMoveComp;
	class BoxComponent* pBoxComp;
	class SkeletalMeshComponent* pSkeleMeshComp;

	SDL_Scancode rightKey = SDL_SCANCODE_D;
	SDL_Scancode leftKey = SDL_SCANCODE_A;
	SDL_Scancode punchKey = SDL_SCANCODE_S;

	int controllerNum = 0;

	int pHealth = 3;

	float sideSpeed;

	float lockedFrames;
};