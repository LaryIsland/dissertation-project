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

	
	SetPosition(Vector3(-1400.f, (Player1->GetPosition().y + Player2->GetPosition().y) / 2, 500.0f));
	SetRotation(Quaternion(0.704f, 0.062f, -0.062f, 0.704f));
	Vector3 cameraPos = GetPosition();
	Vector3 target = GetPosition() + GetForward();
	Vector3 up = Vector3::UnitZ;

	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	GetGame()->GetRenderer()->SetViewMatrix(view);
}