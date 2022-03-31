#include "Mesh.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "Texture.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL_log.h>

namespace {
	union Vertex {
		float f;
		uint8_t b[4];
	};
}

Mesh::Mesh() :
	mVertexArray(nullptr),
	mRadius(0.0f),
	mSpecPower(100.0f),
	mBox(Vector2(Math::Infinity, Math::Infinity), Vector2(Math::NegInfinity, Math::NegInfinity))
{}

Mesh::~Mesh() {}

bool Mesh::Load(const std::string& fileName, Renderer* renderer) {
	std::ifstream file(fileName);
	if (!file.is_open()) {
		SDL_Log("File not found: Mesh %s", fileName.c_str());
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject()) {
		SDL_Log("Mesh %s invalid", fileName.c_str());
		return false;
	}

	mShaderName = doc["shader"].GetString();

	VertexArray::Layout layout = VertexArray::PosNorm;
	size_t vertSize = 8;

	std::string vertexFormat = doc["vertexformat"].GetString();
	if (vertexFormat == "PosNormSkin") {
		layout = VertexArray::PosNormSkin;
		vertSize = 10;
	}

	const rapidjson::Value& textures = doc["textures"];
	if (!textures.IsArray() || textures.Size() < 1) {
		SDL_Log("Mesh %s has no textures", fileName.c_str());
		return false;
	}

	mSpecPower = static_cast<float>(doc["specularPower"].GetDouble());

	for (rapidjson::SizeType i = 0; i < textures.Size(); i++) {
		std::string texName = textures[i].GetString();
		Texture* t = renderer->GetTexture(texName);
		if (t == nullptr) {
			t = renderer->GetTexture(texName);
			if (t == nullptr) {
				t = renderer->GetTexture("Assets/Default.png");
			}
		}
		mTextures.emplace_back(t);
	}

	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1) {
		SDL_Log("Mesh %s missing vertices", fileName.c_str());
		return false;
	}

	std::vector<Vertex> vertices;
	vertices.reserve(vertsJson.Size() * vertSize);
	mRadius = 0.0f;
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++) {
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray()) {
			SDL_Log("Unexpected vertex format for %s", fileName.c_str());
			return false;
		}

		Vector3 pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
		mRadius = Math::Max(mRadius, pos.LengthSq());

		Vector2 AABBpos(vert[0].GetDouble(), vert[1].GetDouble());
		mBox.UpdateMinMax(AABBpos);

		Vertex tmp;
		if (layout == VertexArray::PosNorm) {
			for (rapidjson::SizeType i = 0; i < vert.Size(); i++) {
				tmp.f = static_cast<float>(vert[i].GetDouble());
				vertices.emplace_back(tmp);
			}
		}
		else if (layout == VertexArray::PosNormSkin) {
			for (rapidjson::SizeType j = 0; j < 6; j++) {
				tmp.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(tmp);
			}
			
			for (rapidjson::SizeType j = 6; j < 14; j += 4) {
				tmp.b[0] = vert[j].GetUint();
				tmp.b[1] = vert[j + 1].GetUint();
				tmp.b[2] = vert[j + 2].GetUint();
				tmp.b[3] = vert[j + 3].GetUint();
				vertices.emplace_back(tmp);
			}

			for (rapidjson::SizeType j = 14; j < vert.Size(); j++)
			{
				tmp.f = vert[j].GetDouble();
				vertices.emplace_back(tmp);
			}
		}
	}

	mRadius = Math::Sqrt(mRadius);

	const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1) {
		SDL_Log("Mesh %s has no indices", fileName.c_str());
		return false;
	}

	std::vector<unsigned int> indices;
	indices.reserve(indJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indJson.Size(); i++) {
		const rapidjson::Value& ind = indJson[i];
		if (!ind.IsArray() || ind.Size() != 3) {
			SDL_Log("Invalid indices for %s", fileName.c_str());
			return false;
		}

		indices.emplace_back(ind[0].GetUint());
		indices.emplace_back(ind[1].GetUint());
		indices.emplace_back(ind[2].GetUint());
	}

	mVertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size()) / vertSize, layout, indices.data(), static_cast<unsigned>(indices.size()));
	return true;
}

void Mesh::Unload() {
	delete mVertexArray;
	mVertexArray = nullptr;
}

Texture* Mesh::GetTexture(size_t index) {
	if (index < mTextures.size()) {
		return mTextures[index];
	}
	else {
		return nullptr;
	}
}