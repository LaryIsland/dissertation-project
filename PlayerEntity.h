#pragma once
#include "Entity.h"
#include "SDL_Scancode.h"

class PlayerEntity : public Entity {
public:
	PlayerEntity(class Game* game);

	void UpdateEntity(float deltaTime) override;
	void EntityInput(const InputState& state) override;

	void SetRightKey(SDL_Scancode key) { rightKey = key; }
	void SetLeftKey(SDL_Scancode key) { leftKey = key; }
	void SetUpKey(SDL_Scancode key) { upKey = key; }
	void SetDownKey(SDL_Scancode key) { downKey = key; }
private:
	class MoveComponent* pMoveComp;
	SDL_Scancode rightKey;
	SDL_Scancode leftKey;
	SDL_Scancode upKey;
	SDL_Scancode downKey;
};