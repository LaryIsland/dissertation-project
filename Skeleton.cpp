#include "Skeleton.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL_log.h>

bool Skeleton::Load(const std::string& fileName) {
	std::ifstream file(fileName);
	if (!file.is_open()) {
		SDL_Log("File not found: Skeleton %s", fileName.c_str());
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject()) {
		SDL_Log("Skeleton %s invalid", fileName.c_str());
		return false;
	}

	const rapidjson::Value& boneCount = doc["boneCount"];
	if (!boneCount.IsUint()) {
		SDL_Log("Skeleton %s missing boneCount", fileName.c_str());
		return false;
	}

	size_t count = boneCount.GetUint();

	bBones.reserve(count);

	const rapidjson::Value& bones = doc["bones"];
	if (!bones.IsArray()) {
		SDL_Log("Unexpected bones format for %s", fileName.c_str());
		return false;
	}

	if (bones.Size() != count) {
		SDL_Log("Skeleton %s has a mismatch between boneCount and actual number of bones", fileName.c_str());
		return false;
	}

	Bone temp;
	for (rapidjson::SizeType i = 0; i < count; i++) {
		if (!bones[i].IsObject()) {
			SDL_Log("Skeleton %s: Bone %d invalid.", fileName.c_str(), i);
			return false;
		}

		const rapidjson::Value& name = bones[i]["name"];
		temp.bName = name.GetString();

		const rapidjson::Value& parent = bones[i]["parent"];
		temp.bParent = parent.GetInt();

		const rapidjson::Value& Tpose = bones[i]["Tpose"];
		if (!Tpose.IsObject()) {
			SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
			return false;
		}

		const rapidjson::Value& rot = Tpose["rot"];
		const rapidjson::Value& trans = Tpose["trans"];

		if (!rot.IsArray() || !trans.IsArray()) {
			SDL_Log("Skeleton %s: Bone %d invalid.", fileName.c_str(), i);
			return false;
		}

		temp.bLocalTPose.mRotation.x = rot[0].GetFloat();
		temp.bLocalTPose.mRotation.y = rot[1].GetFloat();
		temp.bLocalTPose.mRotation.z = rot[2].GetFloat();
		temp.bLocalTPose.mRotation.w = rot[3].GetFloat();

		temp.bLocalTPose.mTranslation.x = trans[0].GetFloat();
		temp.bLocalTPose.mTranslation.y = trans[1].GetFloat();
		temp.bLocalTPose.mTranslation.z = trans[2].GetFloat();

		bBones.emplace_back(temp);
	}

	ComputeGlobalInvTPose();

	return true;
}

void Skeleton::ComputeGlobalInvTPose()
{
	bGlobalInvTPose.resize(GetNumBones());
	bGlobalInvTPose[0] = bBones[0].bLocalTPose.ToMatrix();

	for (size_t i = 1; i < bGlobalInvTPose.size(); i++) {
		Matrix4 localMat = bBones[i].bLocalTPose.ToMatrix();
		bGlobalInvTPose[i] = localMat * bGlobalInvTPose[bBones[i].bParent];
	}

	for (size_t i = 0; i < bGlobalInvTPose.size(); i++) {
		bGlobalInvTPose[i].Invert();
	}
}