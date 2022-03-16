#include "Game.h"
#include <algorithm>
#include "Renderer.h"
#include "Entity.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "CameraEntity.h"
#include "PlayerEntity.h"
#include "InputSystem.h"
#include "SDL_scancode.h"
#include <iostream>
using namespace std;

Game::Game() :
	gRenderer(nullptr),
	gIsRunning(true),
	gUpdatingEntities(false)
{}

bool Game::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
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

void Game::UpdateGame()
{
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
}

void Game::GenerateOutput() {
	gRenderer->Draw();
}

void Game::LoadData() {
	/*	Suzanne rotation demo
	Entity* Suzanne = new Entity(this);
	Suzanne->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
	Suzanne->SetScale(100.0f);
	Quaternion q(Vector3::UnitX, Math::TwoPi);
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::Pi + Math::Pi / 4.0f));
	Suzanne->SetRotation(q);
	MeshComponent* meshSuzanne = new MeshComponent(Suzanne);
	MoveComponent* moveSuzanne = new MoveComponent(Suzanne);
	meshSuzanne->SetMesh(gRenderer->GetMesh("Assets/Suzanne.mesh"));
	moveSuzanne->SetAngularSpeed(Math::Pi / 8.0f);
	*/

	// Player 1 loading
	PlayerEntity* Player1 = new PlayerEntity(this);
	Player1->SetPosition(Vector3(-500.0f, 0.0f, 0.0f));
	Player1->SetScale(100.0f);
	Player1->SetRotation(Quaternion(0.5f, -0.5f, -0.5f, -0.5f));
	MeshComponent* meshPlayer1 = new MeshComponent(Player1);
	meshPlayer1->SetMesh(gRenderer->GetMesh("Assets/Suzanne.mesh"));

	// Player 2 loading
	PlayerEntity* Player2 = new PlayerEntity(this);
	Player2->SetPosition(Vector3(500.0f, 0.0f, 0.0f));
	Player2->SetScale(100.0f);
	Player2->SetRotation(Quaternion(-0.5f, -0.5f, -0.5f, 0.5f));
	MeshComponent* meshPlayer2 = new MeshComponent(Player2);
	meshPlayer2->SetMesh(gRenderer->GetMesh("Assets/Suzanne.mesh"));
	Player2->SetRightKey(SDL_SCANCODE_L);
	Player2->SetLeftKey(SDL_SCANCODE_J);
	Player2->SetUpKey(SDL_SCANCODE_I);
	Player2->SetDownKey(SDL_SCANCODE_K);

	Entity* floor = new Entity(this);
	floor->SetPosition(Vector3(0.0f, -100.0f, 200.0f));
	floor->SetScale(200.0f);
	floor->SetRotation(Quaternion(-0.707f, 0.0f, 0.0f, 0.707f));
	MeshComponent* meshFloor = new MeshComponent(floor);
	meshFloor->SetMesh(gRenderer->GetMesh("Assets/Plane.mesh"));

	gRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = gRenderer->GetDirectionalLight();
	dir.lDirection = Vector3(0.0f, -0.707f, -0.707f);
	dir.lDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.lSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	gCameraEntity = new CameraEntity(this);
}

void Game::UnloadData() {
	while (!gEntities.empty()) {
		delete gEntities.back();
	}

	if (gRenderer) {
		gRenderer->UnloadData();
	}
}

void Game::Shutdown() {
	UnloadData();
	if (gRenderer) {
		gRenderer->Shutdown();
	}
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
