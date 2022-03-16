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

	SetPosition(Vector3(0.0f, 100.0f, -1000.0f));
	SetRotation(Quaternion(0.0f, -0.707f, 0.0f, 0.707f));
	Vector3 cameraPos = GetPosition();
	Vector3 target = GetPosition() + GetForward();
	Vector3 up = Vector3::UnitY;

	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	GetGame()->GetRenderer()->SetViewMatrix(view);
}