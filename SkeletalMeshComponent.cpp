#include "SkeletalMeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Entity.h"
#include "Game.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "Animation.h"
#include "Skeleton.h"
#include "Texture.h"

SkeletalMeshComponent::SkeletalMeshComponent(Entity* owner):
	MeshComponent(owner, true),
	mSkeleton(nullptr)
{}

void SkeletalMeshComponent::Draw(Shader* shader) {
	if (mMesh) {
		shader->SetMatrixUniform("uWorldTransform", cOwner->GetWorldTransform());
		shader->SetMatrixUniforms("uMatrixPalette", &mPalette.mEntry[0], MAX_SKELETON_BONES);
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t){ t->SetActive(); }
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

void SkeletalMeshComponent::Update(float deltaTime) {
	if (mAnimation && mSkeleton) {
		mAnimTime += deltaTime * mAnimPlayRate;

		while (mAnimTime > mAnimation->GetDuration()) { mAnimTime -= mAnimation->GetDuration(); }

		ComputeMatrixPalette();
	}
}

float SkeletalMeshComponent::PlayAnimation(const Animation* anim, float playRate) {
	mAnimation = anim;
	mAnimTime = 0.0f;
	mAnimPlayRate = playRate;

	if (!mAnimation) { return 0.0f; }

	ComputeMatrixPalette();

	return mAnimation->GetDuration();
}

void SkeletalMeshComponent::ComputeMatrixPalette() {
	const std::vector<Matrix4>& globalInvBindPoses = mSkeleton->GetGlobalInvTPoses();
	std::vector<Matrix4> currentPoses;
	mAnimation->GetGlobalPoseAtTime(currentPoses, mSkeleton, mAnimTime);

	for (size_t i = 0; i < mSkeleton->GetNumBones(); i++) {
		mPalette.mEntry[i] = globalInvBindPoses[i] * currentPoses[i];
	}
}