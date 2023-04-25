#pragma once
#include <unordered_map>
#include <SDL.h>
#include <string>
#include <vector>
#include <math.h>


class Game {
public:
	Game();
	bool Initialize();
	void GameLoop();
	void Shutdown();

	class Renderer* GetRenderer() { return gRenderer; }
	class WorldPhysics* GetWorldPhysics() { return gWorldPhysics; }
	class CameraEntity* GetCamera() { return gCamera; }
	class HUD* GetHUD() { return gHUD; }

	void AddEntity(class Entity* entity);
	void RemoveEntity(class Entity* entity);
	void PushUI(class UIScreen* screen);

	class Skeleton* GetSkeleton(const std::string& fileName);
	class Animation* GetAnimation(const std::string& fileName);
	const std::vector<class UIScreen*>& GetUIStack() { return gUIStack; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, class Skeleton*> gSkeletons;
	std::unordered_map<std::string, class Animation*> gAnims;

	std::vector<class Entity*> gEntities;
	std::vector<class Entity*> gPendingEntitys;
	std::vector<class UIScreen*> gUIStack;

	class InputSystem* gInputSystem;
	class Renderer* gRenderer;
	class WorldPhysics* gWorldPhysics;
	class CameraEntity* gCamera;
	class HUD* gHUD;

	Uint32 gTicksCount;
	bool gIsRunning;
	bool gUpdatingEntities;

	SDL_Event event;

	class CameraEntity* gCameraEntity;
};