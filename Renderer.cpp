#include "Renderer.h"
#include "Mesh.h"
#include <algorithm>
#include "Shader.h"
#include "VertexArray.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "Texture.h"
#include "Game.h"
#include "UIScreen.h"
#include <glew.h>

Renderer::Renderer(Game* game) :
	gGame(game),
	rMeshShader(nullptr),
	rSkinShader(nullptr)
{}

Renderer::~Renderer() {}

bool Renderer::Initialize(float screenWidth, float screenHeight) {
	rScreenWidth = screenWidth;
	rScreenHeight = screenHeight;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	rWindow = SDL_CreateWindow("Final Gambit", 100, 100, static_cast<int>(rScreenWidth), static_cast<int>(rScreenHeight), SDL_WINDOW_OPENGL);
	if (!rWindow) {
		SDL_Log("CreateWindow Error: %s", SDL_GetError());
		return false;
	}

	rContext = SDL_GL_CreateContext(rWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		SDL_Log("GLEW Error");
		return false;
	}

	glGetError();

	if (!LoadShaders()) {
		SDL_Log("Shaders Error");
		return false;
	}

	CreateUIVerts();

	return true;
}

void Renderer::Shutdown() {
	rMeshShader->Unload();
	delete rMeshShader;
	rSkinShader->Unload();
	delete rSkinShader;
	rUIShader->Unload();
	delete rUIShader;
	SDL_GL_DeleteContext(rContext);
	SDL_DestroyWindow(rWindow);
}

void Renderer::UnloadData() {
	for (auto i : rTextures) {
		i.second->Unload();
		delete i.second;
	}
	rTextures.clear();

	for (auto i : rMeshes) {
		i.second->Unload();
		delete i.second;
	}
	rMeshes.clear();
}

void Renderer::Draw() {
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	rMeshShader->SetActive();
	rMeshShader->SetMatrixUniform("uViewProj", rView * rProjection);
	SetLightUniforms(rMeshShader);
	for (auto mc : rMeshComps) {
		mc->Draw(rMeshShader);
	}

	rSkinShader->SetActive();
	rSkinShader->SetMatrixUniform("uViewProj", rView * rProjection);
	SetLightUniforms(rSkinShader);
	for (auto sk : rSkeleMeshComps) {
		sk->Draw(rSkinShader);
	}

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	rUIShader->SetActive();
	rUIVerts->SetActive();
	for (auto UI : gGame->GetUIStack()) {
		UI->Draw(rUIShader);
	}

	SDL_GL_SwapWindow(rWindow);
}

void Renderer::AddMeshComp(MeshComponent* mesh) {
	if (mesh->GetIsSkeletal()) {
		SkeletalMeshComponent* skeleMesh = static_cast<SkeletalMeshComponent*>(mesh);
		rSkeleMeshComps.emplace_back(skeleMesh);
	} else { rMeshComps.emplace_back(mesh); }
	
}

void Renderer::RemoveMeshComp(MeshComponent* mesh) {
	if (mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* skeleMesh = static_cast<SkeletalMeshComponent*>(mesh);
		auto iter = std::find(rSkeleMeshComps.begin(), rSkeleMeshComps.end(), skeleMesh);
		rSkeleMeshComps.erase(iter);
	}
	else {
		auto iter = std::find(rMeshComps.begin(), rMeshComps.end(), mesh);
		rMeshComps.erase(iter);
	}
}

Mesh* Renderer::GetMesh(const std::string& fileName) {
	Mesh* m = nullptr;
	auto iter = rMeshes.find(fileName);
	if (iter != rMeshes.end()) {
		m = iter->second;
	}
	else {
		m = new Mesh();
		if (m->Load(fileName, this)) {
			rMeshes.emplace(fileName, m);
		}
		else {
			delete m;
			m = nullptr;
		}
	}
	return m;
}

Texture* Renderer::GetTexture(const std::string& fileName) {
	Texture* texture = nullptr;
	auto iter = rTextures.find(fileName);
	if (iter != rTextures.end()) {
		texture = iter->second;
	}
	else {
		texture = new Texture();
		if (texture->Load(fileName)) {
			rTextures.emplace(fileName, texture);
		}
		else {
			delete texture;
			texture = nullptr;
		}
	}
	return texture;
}

bool Renderer::LoadShaders()
{
	rView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	rProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(35.0f), rScreenWidth, rScreenHeight, 25.0f, 10000.0f);
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(rScreenWidth, rScreenHeight);
	//rProjection = Matrix4::CreateOrtho(rScreenWidth, rScreenHeight, 25.0f, 10000.0f);

	//Mesh Shader
	rMeshShader = new Shader();
	if (!rMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag")) { return false; }

	rMeshShader->SetActive();
	rMeshShader->SetMatrixUniform("uViewProj", rView * rProjection);

	//Skin Shader
	rSkinShader = new Shader();
	if (!rSkinShader->Load("Shaders/Skin.vert", "Shaders/Phong.frag")) { return false; }

	rSkinShader->SetActive();
	rSkinShader->SetMatrixUniform("uViewProj", rView * rProjection);

	//UI Shader
	rUIShader = new Shader();
	if (!rUIShader->Load("Shaders/UI.vert", "Shaders/UI.frag")) { return false; }

	rUIShader->SetActive();
	rUIShader->SetMatrixUniform("uViewProj", viewProj);

	return true;
}

void Renderer::CreateUIVerts() {
	float vertices[] = {
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
		0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
		0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	rUIVerts = new VertexArray(vertices, 4, VertexArray::PosNorm, indices, 6);
}

void Renderer::SetLightUniforms(Shader* shader) {
	Matrix4 invView = rView;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
	shader->SetVectorUniform("uAmbientLight", rAmbientLight);
	shader->SetVectorUniform("uDirLight.lDirection",
		rDirectionLight.lDirection);
	shader->SetVectorUniform("uDirLight.lDiffuseColor",
		rDirectionLight.lDiffuseColor);
	shader->SetVectorUniform("uDirLight.lSpecColor",
		rDirectionLight.lSpecColor);
}
