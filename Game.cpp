#include "Game.h"
#include <algorithm>
#include "Renderer.h"
#include "Entity.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "MoveComponent.h"
#include "CameraEntity.h"
#include "PlayerEntity.h"
#include "InputSystem.h"
#include "SDL_scancode.h"
#include <iostream>
#include "Skeleton.h"
#include "Animation.h"
#include "SkeletalMeshComponent.h"
#include "BoxComponent.h"
#include "WorldPhysics.h"
#include "UIScreen.h"
using namespace std;

Game::Game() :
	gRenderer(nullptr),
	gWorldPhysics(nullptr),
	gIsRunning(true),
	gUpdatingEntities(false)
{}

bool Game::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
		SDL_Log("SDL Error: %s", SDL_GetError());
		return false;
	}

	gRenderer = new Renderer(this);
	if (!gRenderer->Initialize(1920.0f, 1080.0f)) {
		SDL_Log("Renderer Error");
		delete gRenderer;
		gRenderer = nullptr;
		return false;
	}

	gInputSystem = new InputSystem();
	if (!gInputSystem->Initialize())
	{
		SDL_Log("InputSystem Error");
		return false;
	}

	gWorldPhysics = new WorldPhysics(this);

	gCamera = new CameraEntity(this);

	LoadData();

	gTicksCount = SDL_GetTicks();

	return true;
}

void Game::GameLoop() {
	while (gIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput() {

	gInputSystem->Update();
	const InputState& state = gInputSystem->GetState();

	if (state.Keyboard.GetKeyState(SDL_SCANCODE_ESCAPE) == RisingEdge) {
		gIsRunning = false;
	}

	for (auto entity : gEntities) {
		entity->ProcessInput(state);
	}
}

void Game::UpdateGame() {
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), gTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - gTicksCount) / 1000.0f;
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}
	gTicksCount = SDL_GetTicks();

	gUpdatingEntities = true;
	for (auto entity : gEntities) {
		entity->Update(deltaTime);
	}
	gUpdatingEntities = false;

	for (auto pending : gPendingEntitys) {
		pending->CalculateWorldTransform();
		gEntities.emplace_back(pending);
	}
	gPendingEntitys.clear();

	std::vector<Entity*> deadEntitys;
	for (auto entity : gEntities) {
		if (entity->GetState() == Entity::Dead) {
			deadEntitys.emplace_back(entity);
		}
	}

	for (auto entity : deadEntitys) {
		delete entity;
	}

	for (auto UI : gUIStack) {
		UI->Update(deltaTime);
	}
}

void Game::GenerateOutput() {
	gRenderer->Draw();
}

void Game::LoadData() {
	// Player 1 loading
	PlayerEntity* Player1 = new PlayerEntity(this);
	Player1->SetPosition(Vector3(-300.0f, -100.0f, 0.0f));
	Player1->SetScale(2.0f);
	Player1->SetRotation(Quaternion(-0.5f, -0.5f, -0.5f, 0.5f));
	SkeletalMeshComponent* skeleMeshPlayer1 = new SkeletalMeshComponent(Player1);
	Mesh* meshPlayer1 = gRenderer->GetMesh("Assets/CatWarrior.mesh");
	skeleMeshPlayer1->SetMesh(meshPlayer1);
	skeleMeshPlayer1->SetSkeleton(Player1->GetGame()->GetSkeleton("Assets/CatWarrior.skele"));
	skeleMeshPlayer1->PlayAnimation(Player1->GetGame()->GetAnimation("Assets/CatActionIdle.anim"));
	Player1->GetBoxComp()->SetObjectBox(meshPlayer1->GetBox());
	//Player1->SetControllerNum(1);

	// Player 2 loading
	PlayerEntity* Player2 = new PlayerEntity(this);
	Player2->SetPosition(Vector3(300.0f, -100.0f, 0.0f));
	Player2->SetScale(2.0f);
	Player2->SetRotation(Quaternion(-0.5f, 0.5f, 0.5f, 0.5f));
	SkeletalMeshComponent* skeleMeshPlayer2 = new SkeletalMeshComponent(Player2);
	Mesh* meshPlayer2 = gRenderer->GetMesh("Assets/CatWarrior.mesh");
	skeleMeshPlayer2->SetMesh(meshPlayer2);
	skeleMeshPlayer2->SetSkeleton(Player2->GetGame()->GetSkeleton("Assets/CatWarrior.skele"));
	skeleMeshPlayer2->PlayAnimation(Player2->GetGame()->GetAnimation("Assets/CatActionIdle.anim"));
	Player2->GetBoxComp()->SetObjectBox(meshPlayer2->GetBox());
	//Player2->SetControllerNum(2);
	Player2->SetRightKey(SDL_SCANCODE_L);
	Player2->SetLeftKey(SDL_SCANCODE_J);
	Player2->SetUpKey(SDL_SCANCODE_I);
	Player2->SetDownKey(SDL_SCANCODE_K);

	Entity* floor = new Entity(this);
	floor->SetPosition(Vector3(0.0f, -100.0f, 0.0f));
	floor->SetScale(300.0f);
	floor->SetRotation(Quaternion(-0.707f, 0.0f, 0.0f, 0.707f));
	MeshComponent* meshFloor = new MeshComponent(floor);
	meshFloor->SetMesh(gRenderer->GetMesh("Assets/Plane.mesh"));

	Entity* leftWall = new Entity(this);
	leftWall->SetPosition(Vector3(-1000.0f, 300.0f, 300.0f));
	leftWall->SetScale(200.0f);
	leftWall->SetRotation(Quaternion(0.0f, 0.643f, 0.0f, 0.766f));
	MeshComponent* meshLeftWall = new MeshComponent(leftWall);
	meshLeftWall->SetMesh(gRenderer->GetMesh("Assets/Plane.mesh"));

	Entity* rightWall = new Entity(this);
	rightWall->SetPosition(Vector3(1000.0f, 300.0f, 300.0f));
	rightWall->SetScale(200.0f);
	rightWall->SetRotation(Quaternion(0.0f, 0.766f, 0.0f, 0.643f));
	MeshComponent* meshRightWall = new MeshComponent(rightWall);
	meshRightWall->SetMesh(gRenderer->GetMesh("Assets/Plane.mesh"));

	gRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = gRenderer->GetDirectionalLight();
	dir.lDirection = Vector3(0.0f, -0.707f, -0.707f);
	dir.lDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.lSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	gCamera->SetPlayers(Player1, Player2);
}

void Game::UnloadData() {
	while (!gEntities.empty()) {
		delete gEntities.back();
	}

	if (gRenderer) {
		gRenderer->UnloadData();
	}

	for (auto skele : gSkeletons) {
		delete skele.second;
	}

	for (auto anim : gAnims) {
		delete anim.second;
	}

}

void Game::Shutdown() {
	UnloadData();
	if (gRenderer) {
		gRenderer->Shutdown();
	}
	delete gWorldPhysics;
	SDL_Quit();
}

void Game::AddEntity(Entity* entity) {
	if (gUpdatingEntities) {
		gPendingEntitys.emplace_back(entity);
	}
	else {
		gEntities.emplace_back(entity);
	}
}

void Game::RemoveEntity(Entity* entity) {
	auto iter = std::find(gPendingEntitys.begin(), gPendingEntitys.end(), entity);
	if (iter != gPendingEntitys.end()) {
		std::iter_swap(iter, gPendingEntitys.end() - 1);
		gPendingEntitys.pop_back();
	}

	iter = std::find(gEntities.begin(), gEntities.end(), entity);
	if (iter != gEntities.end()) {
		std::iter_swap(iter, gEntities.end() - 1);
		gEntities.pop_back();
	}
}

Skeleton* Game::GetSkeleton(const std::string& fileName) {
	auto iter = gSkeletons.find(fileName);
	if (iter != gSkeletons.end()) {
		return iter->second;
	}
	else {
		Skeleton* skele = new Skeleton();
		if (skele->Load(fileName)) {
			gSkeletons.emplace(fileName, skele);
		}
		else {
			delete skele;
			skele = nullptr;
		}
		return skele;
	}
}

Animation* Game::GetAnimation(const std::string& fileName) {
	auto iter = gAnims.find(fileName);
	if (iter != gAnims.end()) {
		return iter->second;
	}
	else {
		Animation* anim = new Animation();
		if (anim->Load(fileName)) {
			gAnims.emplace(fileName, anim);
		}
		else {
			delete anim;
			anim = nullptr;
		}
		return anim;
	}
}

void Game::PushUI(UIScreen* screen) {
	gUIStack.emplace_back(screen);
}