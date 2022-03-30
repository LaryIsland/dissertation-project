#include "UIScreen.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"

UIScreen::UIScreen(Game* game):
	gGame(game),
	mBackground(nullptr),
	mBGPos(0.0f, 250.0f)
{
	gGame->PushUI(this);
}

UIScreen::~UIScreen() {}

void UIScreen::Update(float deltaTime) {}

void UIScreen::Draw(Shader* shader) {
	if (mBackground) {
		DrawTexture(shader, mBackground, mBGPos);
	}
}

void UIScreen::DrawTexture(class Shader* shader, class Texture* texture, const Vector2& offset, float scale) {
	Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(texture->GetWidth()) * scale, static_cast<float>(texture->GetHeight()) * scale, 1.0f);
	Matrix4 transMat = Matrix4::CreateTranslation(Vector3(offset.x, offset.y, 0.0f));
	Matrix4 world = scaleMat * transMat;

	shader->SetMatrixUniform("uWorldTransform", world);
	texture->SetActive();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}