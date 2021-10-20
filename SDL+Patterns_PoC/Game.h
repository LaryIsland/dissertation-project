#pragma once
#include <SDL.h>

class Game {
public:
	Game();

	bool Initialise();

	void Shutdown();

private:
	SDL_Window* gameWindow;
};