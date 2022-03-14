#pragma once
#include "Entity.h"

class PlayerEntity : public Entity {
public:
	PlayerEntity(class Game* game);

	void UpdateEntity(float deltaTime) override;
	void EntityInput(const uint8_t* keys) override;
private:
	class MoveComponent* pMoveComp;
};