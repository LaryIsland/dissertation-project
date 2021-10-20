#include <Game.h>

Game::Game():
	gameWindow(nullptr),
	gameIsRunning(true)
	{}

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

void Game::GameLoop() {
	while (gameIsRunning) {
		ProcessInput();
	}
}

void Game::ProcessInput() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			gameIsRunning = false;
			break;
		}
	}

	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	if (keyboardState[SDL_SCANCODE_ESCAPE]) {gameIsRunning = false;}
}

void Game::Shutdown() {
	SDL_DestroyWindow(gameWindow);
	SDL_Quit();
}