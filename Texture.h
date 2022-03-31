#pragma once
#include <string>

class Texture {
public:
	Texture();
	~Texture();

	bool Load(const std::string& fileName);
	void Unload();
	void CreateFromSurface(struct SDL_Surface* surface);

	void SetActive();

	int GetWidth() const { return tWidth; }
	int GetHeight() const { return tHeight; }

private:
	unsigned int tTextureID;
	int tWidth;
	int tHeight;
};
