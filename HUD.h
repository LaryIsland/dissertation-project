#pragma once
#include "UIScreen.h"
#include <vector>

class HUD : public UIScreen {
public:
	HUD(class Game* game);
	~HUD();

	void Update(float deltaTime) override;
	void Draw(class Shader* shader) override;

protected:
	class Texture* hHealthBar1;
	class Texture* hHealthBar2;
	int hPlayer1;
	int hPlayer2;
};