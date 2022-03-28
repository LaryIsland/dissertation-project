#pragma once
#include "MeshComponent.h"

class SkeletalMeshComponent : public MeshComponent {

public:
	SkeletalMeshComponent(class Entity* owner);

	void Draw(class Shader* shader) override;

	void SetSkeleton(const class Skeleton* skele) { mSkeleton = skele; }

protected:
	const class Skeleton* mSkeleton;
};