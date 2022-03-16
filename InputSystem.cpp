#include "InputSystem.h"
#include <SDL.h>
#include <cstring>

// Keyboard Input
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

// Controller Input
bool ControllerState::GetButtonValue(SDL_GameControllerButton button) const {
	return currButtons[button] == 1;
}

ButtonState ControllerState::GetButtonState(SDL_GameControllerButton button) const {
	if (prevButtons[button] == 0) {
		if (currButtons[button] == 0) {
			return None;
		}
		else {
			return RisingEdge;
		}
	}
	else {
		if (currButtons[button] == 0) {
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

	controllerOne = SDL_GameControllerOpen(0);
	//controllerTwo = SDL_GameControllerOpen(1);

	state.ControllerOne.isConnected = (controllerOne != nullptr);
	memset(state.ControllerOne.currButtons, 0, SDL_CONTROLLER_BUTTON_MAX);
	memset(state.ControllerOne.prevButtons, 0, SDL_CONTROLLER_BUTTON_MAX);
	//state.ControllerTwo.isConnected = (controllerTwo != nullptr);
	//memset(state.ControllerTwo.currButtons, 0, SDL_CONTROLLER_BUTTON_MAX);
	//memset(state.ControllerTwo.prevButtons, 0, SDL_CONTROLLER_BUTTON_MAX);

	return true;
}

void InputSystem::Update() {
	memcpy(state.Keyboard.prevState, state.Keyboard.currState, SDL_NUM_SCANCODES);
	memcpy(state.ControllerOne.prevButtons, state.ControllerOne.currButtons, SDL_CONTROLLER_BUTTON_MAX);
	

	// Controller
	// Buttons
	for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
		state.ControllerOne.currButtons[i] = SDL_GameControllerGetButton(controllerOne, SDL_GameControllerButton(i));
	}

	// Triggers
	state.ControllerOne.leftTrigger = SDL_GameControllerGetAxis(controllerOne, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
	state.ControllerOne.rightTrigger = SDL_GameControllerGetAxis(controllerOne, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

	// Sticks
	int AxisX = 0, AxisY = 0;

	AxisX = SDL_GameControllerGetAxis(controllerOne, SDL_CONTROLLER_AXIS_LEFTX);
	AxisY = -SDL_GameControllerGetAxis(controllerOne, SDL_CONTROLLER_AXIS_LEFTY);
	state.ControllerOne.leftStick = Vector2(AxisX, AxisY);

	AxisX = SDL_GameControllerGetAxis(controllerOne, SDL_CONTROLLER_AXIS_RIGHTX);
	AxisY = -SDL_GameControllerGetAxis(controllerOne, SDL_CONTROLLER_AXIS_RIGHTY);
	state.ControllerOne.rightStick = Vector2(AxisX, AxisY);

	SDL_PumpEvents();
}

void InputSystem::Shutdown() {}