#include "PlayerEntity.h"
#include "MoveComponent.h"
#include "SDL_scancode.h"
#include "Renderer.h"
#include "Game.h"
#include "InputSystem.h"
#include "Collision.h"
#include "CameraEntity.h"
#include "SkeletalMeshComponent.h"
#include "Mesh.h"


PlayerEntity::PlayerEntity(Game* game) : Entity(game) {
	pMoveComp = new MoveComponent(this);
	pBoxComp = new BoxComponent(this);
	pSkeleMeshComp = new SkeletalMeshComponent(this);
	Mesh* meshPlayer = GetGame()->GetRenderer()->GetMesh("Assets/Ninja.mesh");
	pSkeleMeshComp->SetMesh(meshPlayer);
	pSkeleMeshComp->SetSkeleton(GetGame()->GetSkeleton("Assets/Ninja.skele"));
	pBoxComp->SetObjectBox(meshPlayer->GetBox());
	sideSpeed = 300.0f;
	SetPlayerState(Standing);
}

void PlayerEntity::UpdateEntity(float deltaTime) {
	Entity::UpdateEntity(deltaTime);
	FixCollisions();
	if (lockedFrames != 0) { AnimationLock(deltaTime); }
	if (currState == Punching && lockedFrames >= 0.176f && lockedFrames <= 0.256f) {
		if ((abs(pEnemy->GetPosition().y - GetPosition().y) < 220) && pEnemy->GetPlayerState() != HitStun) {
			pEnemy->SetPlayerState(HitStun);
			pEnemy->ReduceHealth();
		}
	}
}

void PlayerEntity::EntityInput(const InputState& state) {
	// Keyboard Input
	switch (state.Keyboard.GetKeyState(rightKey)) {
	case RisingEdge:
	case Holding:
		SetPlayerState(WalkingRight);
		break;
	case FallingEdge:
		SetPlayerState(Standing);
		break;
	}

	switch (state.Keyboard.GetKeyState(leftKey)) {
	case RisingEdge:
	case Holding:
		SetPlayerState(WalkingLeft);
		break;
	case FallingEdge:
		SetPlayerState(Standing);
		break;
	}

	switch (state.Keyboard.GetKeyState(punchKey)) {
	case RisingEdge:
		SetPlayerState(Punching);
		break;
	}


	// Controller Input
	if (controllerNum == 1) {
		if (state.ControllerOne.GetLeftStick().x >= 0.7 * 32768) {
			SetPlayerState(WalkingRight);
		}
		else if (state.ControllerOne.GetLeftStick().x <= -0.7 * 32768) {
			SetPlayerState(WalkingLeft);
		}
		else {
			SetPlayerState(Standing);
		}
	}
	else if (controllerNum == 2) {
		if (state.ControllerTwo.GetLeftStick().x >= 0.7 * 32768) {
			SetPlayerState(WalkingRight);
		}
		else if (state.ControllerTwo.GetLeftStick().x <= -0.7 * 32768) {
			SetPlayerState(WalkingLeft);
		}
		else {
			SetPlayerState(Standing);
		}
	}
}

void PlayerEntity::SetPlayerState(PlayerState playerState) {
	switch (playerState) {
	case Standing:
		if (currState != Victory && currState != Defeat && lockedFrames == 0) {
			currState = Standing;
			pMoveComp->SetSideSpeed(0);
			pSkeleMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/NinjaIdle.anim"));
		}
		break;
	case WalkingRight:
		if ((currState == Standing || currState == Punching) && lockedFrames == 0) {
			currState = WalkingRight;
			pMoveComp->SetSideSpeed(sideSpeed);
			if (GetPosition().y < GetGame()->GetCamera()->GetPlayer1()->GetPosition().y ||
				GetPosition().y < GetGame()->GetCamera()->GetPlayer2()->GetPosition().y) {
				pSkeleMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/NinjaWalkingForward.anim"), 1.2f);
			}
			else { pSkeleMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/NinjaWalkingBackward.anim"), 1.2f); }
		}
		break;
	case WalkingLeft:
		if ((currState == Standing || currState == Punching) && lockedFrames == 0) {
			currState = WalkingLeft;
			pMoveComp->SetSideSpeed(-sideSpeed);
			if (GetPosition().y > GetGame()->GetCamera()->GetPlayer1()->GetPosition().y ||
				GetPosition().y > GetGame()->GetCamera()->GetPlayer2()->GetPosition().y) {
				pSkeleMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/NinjaWalkingForward.anim"), 1.2f);
			}
			else { pSkeleMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/NinjaWalkingBackward.anim"), 1.2f); }
		}
		break;
	case Punching:
		if ((currState == Standing || currState == WalkingLeft || currState == WalkingRight) && lockedFrames == 0) {
			currState = Punching;
			pMoveComp->SetSideSpeed(0);
			pSkeleMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/NinjaPunch.anim"), 2.0f);
			AnimationLock(0.01f);
		}
		break;
	case HitStun:
		currState = HitStun;
		if (GetPosition().y > GetGame()->GetCamera()->GetPlayer1()->GetPosition().y ||
			GetPosition().y > GetGame()->GetCamera()->GetPlayer2()->GetPosition().y) {
			pMoveComp->SetSideSpeed(sideSpeed);
		} else { pMoveComp->SetSideSpeed(-sideSpeed); }
		pSkeleMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/NinjaHitStun.anim"), 2.0f);
		AnimationLock(0.01f);
		break;
	case Victory:
		currState = Victory;
		pSkeleMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/NinjaVictory.anim"), 1.0f);
		break;
	case Defeat:
		currState = Defeat;
		pSkeleMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/NinjaDefeat.anim"), 1.0f);
		break;
	}
}

bool PlayerEntity::AnimationLock(float deltaTime) {
	if (lockedFrames >= 0.512f) {
		lockedFrames = 0.f;
		SetPlayerState(Standing);
		return false;
	}
	else {
		lockedFrames += deltaTime;
		return true;
	}
}

void PlayerEntity::FixCollisions() {
	CalculateWorldTransform();

	const AABB& playerBox = pBoxComp->GetWorldBox();
	Vector3 pos = GetPosition();

	const AABB& enemyBox = pEnemy->GetBoxComp()->GetWorldBox();
	if (Intersect(playerBox, enemyBox)) {
		float dy1 = enemyBox.aMax.y - playerBox.aMin.y;
		float dy2 = enemyBox.aMin.y - playerBox.aMax.y;

		float dy = Math::Abs(dy1) < Math::Abs(dy2) ? dy1 : dy2;

		pos.y += dy;

		SetPosition(pos);
		pBoxComp->OnUpdateWorldTransform();
	}
}