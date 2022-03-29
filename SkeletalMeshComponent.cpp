#include "SkeletalMeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Entity.h"
#include "Game.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "Animation.h"
#include "Skeleton.h"

SkeletalMeshComponent::SkeletalMeshComponent(Entity* owner):
	MeshComponent(owner, true),
	mSkeleton(nullptr)
{}

void SkeletalMeshComponent::ComputeMatrixPalette() {
	const std::vector<Matrix4>& globalInvBindPoses = mSkeleton->GetGlobalInvTPoses();
	std::vector<Matrix4> currentPoses;
	mAnimation->GetGlobalPoseAtTime(currentPoses, mSkeleton, mAnimTime);

	for (size_t i = 0; i < mSkeleton->GetNumBones(); i++) {
		mPalette.mEntry[i] = globalInvBindPoses[i] * currentPoses[i];
	}
}