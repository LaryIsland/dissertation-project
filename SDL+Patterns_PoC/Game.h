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

	bool gameIsRunning;

	void ProcessInput();
	void GenerateFrame();
};