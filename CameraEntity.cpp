#include "CameraEntity.h"
#include "MoveComponent.h"
#include "SDL_scancode.h"
#include "Renderer.h"
#include "Game.h"

CameraEntity::CameraEntity(Game* game) : Entity(game) {
	cMoveComp = new MoveComponent(this);
}

void CameraEntity::UpdateEntity(float deltaTime) {
	Entity::UpdateEntity(deltaTime);

	Vector3 cameraPos = GetPosition();
	Vector3 target = GetPosition() + GetForward() * 100.0f;
	Vector3 up = Vector3::UnitZ;

	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	GetGame()->GetRenderer()->SetViewMatrix(view);
}