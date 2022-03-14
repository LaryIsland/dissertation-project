#include "Game.h"

int main(int argc, char* argv[]) {
	Game game;
	bool initResult = game.Initialize();
	if (initResult) {
		game.GameLoop();
	}
	game.Shutdown();
	return 0;
}
