#pragma once
#include <SDL.h>

class Game {
public:
	Game();

	bool Initialise();

	void GameLoop();

	void Shutdown();

private:
	SDL_Window* gameWindow;
	SDL_Renderer* gameRenderer;
	Uint32 gameTickCount;
	bool gameIsRunning;
	float windowCol[4];

	void ProcessInput();
	void GenerateFrame();
	void Update();
};