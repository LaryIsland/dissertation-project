#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SDL.h>
#include "Math.h"


struct DirectionalLight {
	Vector3 lDirection;
	Vector3 lDiffuseColor;
	Vector3 lSpecColor;
};

class Renderer {
public:
	Renderer(class Game* game);
	~Renderer();

	bool Initialize(float screenWidth, float screenHeight);
	void Shutdown();
	void UnloadData();

	void Draw();

	void AddMeshComp(class MeshComponent* mesh);
	void RemoveMeshComp(class MeshComponent* mesh);

	class Mesh* GetMesh(const std::string& fileName);
	class Texture* GetTexture(const std::string& fileName);

	void SetViewMatrix(const Matrix4& view) { rView = view; }

	void SetAmbientLight(const Vector3& ambient) { rAmbientLight = ambient; }
	DirectionalLight& GetDirectionalLight() { return rDirectionLight; }

	float GetScreenWidth() const { return rScreenWidth; }
	float GetScreenHeight() const { return rScreenHeight; }
private:
	bool LoadShaders();
	void SetLightUniforms(class Shader* shader);
	void CreateUIVerts();

	std::unordered_map<std::string, class Mesh*> rMeshes;
	std::unordered_map<std::string, class Texture*> rTextures;

	std::vector<class MeshComponent*> rMeshComps;
	std::vector<class SkeletalMeshComponent*> rSkeleMeshComps;

	class Game* gGame;

	class Shader* rMeshShader;
	class Shader* rSkinShader;
	class Shader* rUIShader;
	class VertexArray* rUIVerts;

	Matrix4 rView;
	Matrix4 rProjection;
	float rScreenWidth;
	float rScreenHeight;

	Vector3 rAmbientLight;
	DirectionalLight rDirectionLight;

	SDL_Window* rWindow;
	SDL_GLContext rContext;
};