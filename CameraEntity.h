#pragma once
#include "Entity.h"

class CameraEntity : public Entity {
public:
	CameraEntity(class Game* game);

	void UpdateEntity(float deltaTime) override;
private:
	class MoveComponent* cMoveComp;
};