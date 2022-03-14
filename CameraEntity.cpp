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

void CameraEntity::EntityInput(const uint8_t* keys) {
	float forwardSpeed = 0.0f;
	float sideSpeed = 0.0f;
	float angularSpeed = 0.0f;

	if (keys[SDL_SCANCODE_W]) {
		forwardSpeed += 300.0f;
	}
	if (keys[SDL_SCANCODE_S]) {
		forwardSpeed -= 300.0f;
	}
	if (keys[SDL_SCANCODE_A]) {
		sideSpeed -= 300.0f;
	}
	if (keys[SDL_SCANCODE_D]) {
		sideSpeed += 300.0f;
	}
	if (keys[SDL_SCANCODE_Q]) {
		angularSpeed -= Math::Pi;
	}
	if (keys[SDL_SCANCODE_E]) {
		angularSpeed += Math::Pi;
	}

	cMoveComp->SetForwardSpeed(forwardSpeed);
	cMoveComp->SetSideSpeed(sideSpeed);
	cMoveComp->SetAngularSpeed(angularSpeed);
}