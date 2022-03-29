#pragma once
#include "BoneTransform.h"
#include <vector>
#include <string>

class Animation {
public:
	bool Load(const std::string& fileName);

	size_t GetNumBones() const { return aNumBones; }
	size_t GetNumFrames() const { return aNumFrames; }
	float GetDuration() const { return aDuration; }
	float GetFrameDuration() const { return aFrameDuration; }

	void GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const class Skeleton* inSkeleton, float inTime) const;

private:
	size_t aNumBones;
	size_t aNumFrames;
	float aDuration;
	float aFrameDuration;

	std::vector<std::vector<BoneTransform>> aTracks;
};