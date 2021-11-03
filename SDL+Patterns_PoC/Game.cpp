#include <Game.h>

Game::Game():
	gameWindow(nullptr),
	gameIsRunning(true),
	gameTickCount(0),
	windowCol{0, 128, 255, 255}
	{}

// Returns true if game initialises
// correctly, false otherwise
bool Game::Initialise() {

	// Initialise SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Create an SDL Window
	// Args-
	// Title, x-coord, y-coord, width, height, flags
	gameWindow = SDL_CreateWindow("SDL Proof of Concept", 100, 100, 1280, 720, 0);

	if (!gameWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// Create an SDL renderer
	// Args-
	// SDL_Window, driver index, flags
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

// Checks for current user input 
void Game::ProcessInput() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		// If recieving an SDL_QUIT event, end GameLoop
		if (event.type == SDL_QUIT) {
			gameIsRunning = false;
			break;
		}
	}

	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	// If ESCAPE key is pressed, end GameLoop
	if (keyboardState[SDL_SCANCODE_ESCAPE]) {gameIsRunning = false;}
}

void Game::GenerateFrame() {
	SDL_SetRenderDrawColor(gameRenderer, (int)windowCol[0], (int)windowCol[1], (int)windowCol[2], (int)windowCol[3]);

	SDL_RenderClear(gameRenderer);

	SDL_RenderPresent(gameRenderer);
}

// Updates the game logic
void Game::Update() {

	// Waits until enough ticks have passed
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), gameTickCount + 16));

	// Gets the amount of real time passed since last tick
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