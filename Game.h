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

	void AddEntity(class Entity* entity);
	void RemoveEntity(class Entity* entity);

	class Skeleton* GetSkeleton(const std::string& fileName);

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, class Skeleton*> gSkeletons;

	std::vector<class Entity*> gEntities;
	std::vector<class Entity*> gPendingEntitys;

	class InputSystem* gInputSystem;
	class Renderer* gRenderer;

	Uint32 gTicksCount;
	bool gIsRunning;
	bool gUpdatingEntities;

	class CameraEntity* gCameraEntity;
};