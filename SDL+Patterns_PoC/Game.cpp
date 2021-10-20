#include <Game.h>

Game::Game():gameWindow(nullptr) {}

bool Game::Initialise() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	gameWindow = SDL_CreateWindow("SDL Proof of Concept", 100, 100, 1280, 720, 0);

	if (!gameWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	return true;
}