#pragma once
#include <SDL.h>

class Game {
public:
	Game();

	bool Initialise();

private:
	SDL_Window* gameWindow;
};