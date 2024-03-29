#pragma once
#include "MeshComponent.h"

const size_t MAX_SKELETON_BONES = 96;

struct MatrixPalette {
	Matrix4 mEntry[MAX_SKELETON_BONES];
};

class SkeletalMeshComponent : public MeshComponent {
public:

	SkeletalMeshComponent(class Entity* owner);

	void Draw(class Shader* shader) override;

	void Update(float deltaTime) override;

	void SetSkeleton(const class Skeleton* skele) { mSkeleton = skele; }

	float PlayAnimation(const class Animation* anim, float playRate = 1.0f);

protected:
	void ComputeMatrixPalette();

	MatrixPalette mPalette;

	const class Skeleton* mSkeleton;
	const class Animation* mAnimation;

	float mAnimPlayRate;
	float mAnimTime;
};