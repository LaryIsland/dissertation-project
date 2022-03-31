#include "HUD.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include <algorithm>

HUD::HUD(Game* game): UIScreen(game) {
	Renderer* renderer = gGame->GetRenderer();
	hHealthBar = renderer->GetTexture("Assets/HealthBar.png");
}

HUD::~HUD() {}

void HUD::Update(float deltaTime) {
	UIScreen::Update(deltaTime);
}

void HUD::Draw(Shader* shader) {
	DrawTexture(shader, hHealthBar, Vector2(-350.0f, -350.0f));
}