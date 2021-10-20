#include <Game.h>

int main(int argc, char* argv[]) {
	Game game;
	bool initResult = game.Initialise();
	game.Shutdown();
	return 0;
}