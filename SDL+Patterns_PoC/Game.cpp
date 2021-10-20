#include <Game.h>

Game::Game() {}

bool Game::Initialise() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	return true;
}