#pragma once
#include "Entity.h"

class CameraEntity : public Entity {
public:
	CameraEntity(class Game* game);

	void UpdateEntity(float deltaTime) override;
	void EntityInput(const uint8_t* keys) override;
private:
	class MoveComponent* cMoveComp;
};