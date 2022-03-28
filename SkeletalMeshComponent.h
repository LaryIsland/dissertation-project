#pragma once
#include "MeshComponent.h"

class SkeletalMeshComponent : public MeshComponent {

public:
	SkeletalMeshComponent(class Entity* owner);

	void Draw(class Shader* shader) override;
};