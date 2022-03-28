#include "Renderer.h"
#include "Mesh.h"
#include <algorithm>
#include "Shader.h"
#include "VertexArray.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
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

	rWindow = SDL_CreateWindow("Animation Proof of Concept", 100, 100, static_cast<int>(rScreenWidth), static_cast<int>(rScreenHeight), SDL_WINDOW_OPENGL);
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

	return true;
}

void Renderer::Shutdown() {
	rMeshShader->Unload();
	delete rMeshShader;
	rSkinShader->Unload();
	delete rSkinShader;
	SDL_GL_DeleteContext(rContext);
	SDL_DestroyWindow(rWindow);
}

void Renderer::UnloadData() {
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

bool Renderer::LoadShaders()
{
	rView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	rProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(30.0f), rScreenWidth, rScreenHeight, 25.0f, 10000.0f);
	//rProjection = Matrix4::CreateOrtho(rScreenWidth, rScreenHeight, 25.0f, 10000.0f);

	rMeshShader = new Shader();
	if (!rMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag")) { return false; }

	rMeshShader->SetActive();
	rMeshShader->SetMatrixUniform("uViewProj", rView * rProjection);


	rSkinShader = new Shader();
	if (!rSkinShader->Load("Shaders/Skin.vert", "Shaders/Phong.frag")) { return false; }

	rSkinShader->SetActive();
	rSkinShader->SetMatrixUniform("uViewProj", rView * rProjection);

	return true;
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
