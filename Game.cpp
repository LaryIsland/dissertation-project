#include "Game.h"
#include <algorithm>
#include "Renderer.h"
#include "Entity.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "CameraEntity.h"
#include "PlayerEntity.h"

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
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			gIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) {
		gIsRunning = false;
	}

	for (auto actor : gEntities) {
		actor->ProcessInput(state);
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
	for (auto actor : gEntities) {
		actor->Update(deltaTime);
	}
	gUpdatingEntities = false;

	for (auto pending : gPendingEntitys) {
		pending->CalculateWorldTransform();
		gEntities.emplace_back(pending);
	}
	gPendingEntitys.clear();

	std::vector<Entity*> deadEntitys;
	for (auto actor : gEntities) {
		if (actor->GetState() == Entity::Dead) {
			deadEntitys.emplace_back(actor);
		}
	}

	for (auto actor : deadEntitys) {
		delete actor;
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
	Player1->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
	Player1->SetScale(100.0f);
	Quaternion q(Vector3::UnitX, Math::TwoPi);
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::Pi + Math::Pi / 4.0f));
	Player1->SetRotation(q);
	MeshComponent* meshPlayer1 = new MeshComponent(Player1);
	MoveComponent* movePlayer1 = new MoveComponent(Player1);
	meshPlayer1->SetMesh(gRenderer->GetMesh("Assets/Suzanne.mesh"));

	Entity* floor = new Entity(this);
	floor->SetPosition(Vector3(200.0f, 0.0f, -150.0f));
	floor->SetScale(500.0f);
	floor->SetRotation(q);
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

void Game::AddEntity(Entity* actor) {
	if (gUpdatingEntities) {
		gPendingEntitys.emplace_back(actor);
	}
	else {
		gEntities.emplace_back(actor);
	}
}

void Game::RemoveEntity(Entity* actor) {
	auto iter = std::find(gPendingEntitys.begin(), gPendingEntitys.end(), actor);
	if (iter != gPendingEntitys.end()) {
		std::iter_swap(iter, gPendingEntitys.end() - 1);
		gPendingEntitys.pop_back();
	}

	iter = std::find(gEntities.begin(), gEntities.end(), actor);
	if (iter != gEntities.end()) {
		std::iter_swap(iter, gEntities.end() - 1);
		gEntities.pop_back();
	}
}
