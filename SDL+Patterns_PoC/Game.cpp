#include <Game.h>

Game::Game():
	gameWindow(nullptr),
	gameIsRunning(true),
	gameTickCount(0),
	windowCol{0, 128, 255, 255}
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

	gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);

	if (!gameRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	return true;
}

void Game::GameLoop() {
	while (gameIsRunning) {
		ProcessInput();
		GenerateFrame();
		Update();
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

void Game::GenerateFrame() {
	SDL_SetRenderDrawColor(gameRenderer, (int)windowCol[0], (int)windowCol[1], (int)windowCol[2], (int)windowCol[3]);

	SDL_RenderClear(gameRenderer);

	SDL_RenderPresent(gameRenderer);
}

void Game::Update() {
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), gameTickCount + 16));

	float stepTime = (SDL_GetTicks() - gameTickCount) / 1000.0f;

	if (stepTime > 0.05f) {stepTime = 0.05f;}

	gameTickCount = SDL_GetTicks();

	for (auto &col : windowCol) {
		col += stepTime * 50;
		if (col > 255.0) {col -= 255;}
	}
}

void Game::Shutdown() {
	SDL_DestroyWindow(gameWindow);
	SDL_DestroyRenderer(gameRenderer);
	SDL_Quit();
}