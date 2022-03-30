#pragma once
#include "Entity.h"
#include "PlayerEntity.h"

class CameraEntity : public Entity {
public:
	CameraEntity(class Game* game);

	void UpdateEntity(float deltaTime) override;

	void SetPlayers(PlayerEntity* PlayerEntity1, PlayerEntity* PlayerEntity2) { Player1 = PlayerEntity1; Player2 = PlayerEntity2; }
	PlayerEntity* GetPlayer1() { return Player1; }
	PlayerEntity* GetPlayer2() { return Player2; }
private:
	class MoveComponent* cMoveComp;
	class PlayerEntity* Player1;
	class PlayerEntity* Player2;
};