#include "Animation.h"
#include "Skeleton.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL_log.h>


bool Animation::Load(const std::string& fileName) {
	std::ifstream file(fileName);
	if (!file.is_open()) {
		SDL_Log("File not found: Animation %s", fileName.c_str());
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject()) {
		SDL_Log("Animation %s invalid", fileName.c_str());
		return false;
	}

	const rapidjson::Value& sequence = doc["sequence"];
	if (!sequence.IsObject()) {
		SDL_Log("Animation %s missing sequence.", fileName.c_str());
		return false;
	}

	const rapidjson::Value& frames = sequence["frames"];
	const rapidjson::Value& length = sequence["length"];
	const rapidjson::Value& boneCount = sequence["boneCount"];

	if (!frames.IsUint() || !length.IsDouble() || !boneCount.IsUint()) {
		SDL_Log("Unexpected frame, length, or boneCount format for %s", fileName.c_str());
		return false;
	}

	aNumFrames = frames.GetUint();
	aDuration = length.GetFloat();
	aNumBones = boneCount.GetUint();
	aFrameDuration = aDuration / (aNumFrames - 1);

	aTracks.resize(aNumBones);

	const rapidjson::Value& tracks = sequence["tracks"];

	if (!tracks.IsArray()) {
		SDL_Log("Sequence %s missing tracks", fileName.c_str());
		return false;
	}

	for (rapidjson::SizeType i = 0; i < tracks.Size(); i++) {
		if (!tracks[i].IsObject()) {
			SDL_Log("Animation %s: Track element %d invalid.", fileName.c_str(), i);
			return false;
		}

		size_t boneIndex = tracks[i]["bone"].GetUint();

		const rapidjson::Value& transforms = tracks[i]["transforms"];
		if (!transforms.IsArray()) {
			SDL_Log("Animation %s: Track element %d missing transforms.", fileName.c_str(), i);
			return false;
		}

		BoneTransform tmp;
		if (transforms.Size() < aNumFrames) {
			SDL_Log("Animation %s: Track element %d has a mismatch between numFrames and actual number of frames", fileName.c_str(), i);
			return false;
		}

		for (rapidjson::SizeType j = 0; j < transforms.Size(); j++) {
			const rapidjson::Value& rot = transforms[j]["rot"];
			const rapidjson::Value& trans = transforms[j]["trans"];

			if (!rot.IsArray() || !trans.IsArray()) {
				SDL_Log("Skeleton %s: Bone %d invalid.", fileName.c_str(), i);
				return false;
			}

			tmp.mRotation.x = rot[0].GetFloat();
			tmp.mRotation.y = rot[1].GetFloat();
			tmp.mRotation.z = rot[2].GetFloat();
			tmp.mRotation.w = rot[3].GetFloat();

			tmp.mTranslation.x = trans[0].GetFloat();
			tmp.mTranslation.y = trans[1].GetFloat();
			tmp.mTranslation.z = trans[2].GetFloat();

			aTracks[boneIndex].emplace_back(tmp);
		}
	}

	return true;
}

void Animation::GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const Skeleton* inSkeleton, float inTime) const {
	if (outPoses.size() != aNumBones) {
		outPoses.resize(aNumBones);
	}

	size_t frame = static_cast<size_t>(inTime / aFrameDuration);
	size_t nextFrame = frame + 1;
	float pct = inTime / aFrameDuration - frame;

	if (aTracks[0].size() > 0) {
		BoneTransform interp = BoneTransform::Interpolate(aTracks[0][frame],
			aTracks[0][nextFrame], pct);
		outPoses[0] = interp.ToMatrix();
	}
	else {
		outPoses[0] = Matrix4::Identity;
	}

	const std::vector<Skeleton::Bone>& bones = inSkeleton->GetBones();

	for (size_t bone = 1; bone < aNumBones; bone++) {
		Matrix4 localMat;
		if (aTracks[bone].size() > 0) {
			BoneTransform interp = BoneTransform::Interpolate(aTracks[bone][frame],
				aTracks[bone][nextFrame], pct);
			localMat = interp.ToMatrix();
		}

		outPoses[bone] = localMat * outPoses[bones[bone].bParent];
	}
}