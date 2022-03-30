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

	void AddEntity(class Entity* entity);
	void RemoveEntity(class Entity* entity);

	class Skeleton* GetSkeleton(const std::string& fileName);
	class Animation* GetAnimation(const std::string& fileName);

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

	class InputSystem* gInputSystem;
	class Renderer* gRenderer;
	class WorldPhysics* gWorldPhysics;
	class CameraEntity* gCamera;

	Uint32 gTicksCount;
	bool gIsRunning;
	bool gUpdatingEntities;

	class CameraEntity* gCameraEntity;
};