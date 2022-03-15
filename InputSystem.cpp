#include "InputSystem.h"
#include <SDL.h>
#include <cstring>

bool KeyboardState::GetKeyValue(SDL_Scancode keyCode) const {
	return currState[keyCode] == 1;
}

ButtonState KeyboardState::GetKeyState(SDL_Scancode keyCode) const {
	if (prevState[keyCode] == 0) {
		if (currState[keyCode] == 0) {
			return None;
		}
		else {
			return RisingEdge;
		}
	}
	else {
		if (currState[keyCode] == 0) {
			return FallingEdge;
		}
		else {
			return Holding;
		}
	}
}

bool InputSystem::Initialize() {
	state.Keyboard.currState = SDL_GetKeyboardState(NULL);
	memset(state.Keyboard.prevState, 0, SDL_NUM_SCANCODES);

	return true;
}

void InputSystem::Update() {
	memcpy(state.Keyboard.prevState, state.Keyboard.currState, SDL_NUM_SCANCODES);
	SDL_PumpEvents();
}

void InputSystem::Shutdown() {}