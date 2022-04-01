#include "HUD.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "CameraEntity.h"
#include <algorithm>

HUD::HUD(Game* game): UIScreen(game) {
	Renderer* renderer = gGame->GetRenderer();
	hHealthBar1 = renderer->GetTexture("Assets/Health3.png");
	hHealthBar2 = renderer->GetTexture("Assets/Health3.png");
}

HUD::~HUD() {}

void HUD::Update(float deltaTime) {
	UIScreen::Update(deltaTime);
	int tmp = gGame->GetCamera()->GetPlayer1()->GetHealth();
	if (hPlayer1 != tmp) {
		switch (tmp) {
		case 2:
			hHealthBar1 = gGame->GetRenderer()->GetTexture("Assets/Health2.png");
			hPlayer1 = tmp;
			break;
		case 1:
			hHealthBar1 = gGame->GetRenderer()->GetTexture("Assets/Health1.png");
			hPlayer1 = tmp;
			break;
		}
	}

	tmp = gGame->GetCamera()->GetPlayer2()->GetHealth();
	if (hPlayer2 != tmp) {
		switch (tmp) {
		case 2:
			hHealthBar2 = gGame->GetRenderer()->GetTexture("Assets/Health2.png");
			hPlayer2 = tmp;
			break;
		case 1:
			hHealthBar2 = gGame->GetRenderer()->GetTexture("Assets/Health1.png");
			hPlayer2 = tmp;
			break;
		}
	}
}

void HUD::Draw(Shader* shader) {
	DrawTexture(shader, hHealthBar1, Vector2(-600.0f, 400.0f));
	DrawTexture(shader, hHealthBar2, Vector2(600.0f, 400.0f));
}