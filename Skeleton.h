#pragma once
#include <string>
#include <vector>
#include "Math.h"
#include "BoneTransform.h"

class Skeleton {
public:
	struct Bone {
		BoneTransform bLocalTPose;
		std::string bName;
		int bParent;
	};

	bool Load(const std::string& fileName);

	size_t GetNumBones() const { return bBones.size(); }
	const Bone& GetBone(size_t idx) const { return bBones[idx]; }
	const std::vector<Bone>& GetBones() const { return bBones; }
	const std::vector<Matrix4>& GetGlobalInvTPoses() const { return bGlobalInvTPose; }

protected:
	void ComputeGlobalInvTPose();

private:
	std::vector<Bone> bBones;
	std::vector<Matrix4> bGlobalInvTPose;
};