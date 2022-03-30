#pragma once
#include "Math.h"
#include <cstdint>
#include <string>
#include <functional>
#include <vector>


class UIScreen {
public:
	UIScreen(class Game* game);
	virtual ~UIScreen();

	virtual void Update(float deltaTime);
	virtual void Draw(class Shader* shader);

protected:
	void DrawTexture(class Shader* shader, class Texture* texture, const Vector2& offset = Vector2::Zero, float scale = 1.0f);

	class Game* gGame;
	class Texture* mBackground;

	Vector2 mBGPos;
};