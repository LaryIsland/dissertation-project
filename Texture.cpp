#include "Texture.h"
#include <SOIL2.h>
#define GLEW_STATIC
#include <glew.h>
#include <SDL.h>

Texture::Texture():
	tTextureID(0),
	tWidth(0),
	tHeight(0)
{}

Texture::~Texture() {}

bool Texture::Load(const std::string& fileName) {
	int channels = 0;

	unsigned char* image = SOIL_load_image(fileName.c_str(), &tWidth, &tHeight, &channels, SOIL_LOAD_AUTO);

	if (image == nullptr) {
		SDL_Log("SOIL failed loading image %s: %s", fileName.c_str(), SOIL_last_result());
		return false;
	}

	int format = GL_RGB;
	if (channels == 4) {
		format = GL_RGBA;
	}

	glGenTextures(1, &tTextureID);
	glBindTexture(GL_TEXTURE_2D, tTextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, format, tWidth, tHeight, 0, format, GL_UNSIGNED_BYTE, image);

	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture::Unload() { glDeleteTextures(1, &tTextureID); }

void Texture::CreateFromSurface(SDL_Surface* surface) {
	tWidth = surface->w;
	tHeight = surface->h;

	glGenTextures(1, &tTextureID);
	glBindTexture(GL_TEXTURE_2D, tTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tWidth, tHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::SetActive() {
	glBindTexture(GL_TEXTURE_2D, tTextureID);
}