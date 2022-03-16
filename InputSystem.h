#pragma once
#include <SDL.h>
#include "Math.h"

enum ButtonState {
	None,
	RisingEdge,
	FallingEdge,
	Holding
};

class KeyboardState {
public:
	friend class InputSystem;

	bool GetKeyValue(SDL_Scancode keyCode) const;
	ButtonState GetKeyState(SDL_Scancode keyCode) const;

private:
	const Uint8* currState;
	Uint8 prevState[SDL_NUM_SCANCODES];
};

class ControllerState {
public:
	friend class InputSystem;

	// Face buttons
	bool GetButtonValue(SDL_GameControllerButton button) const;
	ButtonState GetButtonState(SDL_GameControllerButton button) const;

	const Vector2& GetLeftStick() const { return leftStick; }
	const Vector2& GetRightStick() const { return rightStick; }
	float GetLeftTrigger() const { return leftTrigger; }
	float GetRightTrigger() const { return rightTrigger; }

	bool IsConnected() const { return isConnected; }

private:
	Uint8 currButtons[SDL_CONTROLLER_BUTTON_MAX];
	Uint8 prevButtons[SDL_CONTROLLER_BUTTON_MAX];
	Vector2 leftStick;
	Vector2 rightStick;
	float leftTrigger;
	float rightTrigger;
	bool isConnected;
};

struct InputState {
	KeyboardState Keyboard;
	ControllerState ControllerOne;
	ControllerState ControllerTwo;
};

class InputSystem {
public:
	bool Initialize();
	void Shutdown();

	void Update();

	const InputState& GetState() const { return state; }

private:
	InputState state;
	SDL_GameController* controllerOne;
	SDL_GameController* controllerTwo;
};