#pragma once
#include "Component.h"

class MeshComponent : public Component {
public:
	MeshComponent(class Entity* owner, bool isSkeletal = false);
	~MeshComponent();
	virtual void Draw(class Shader* shader);
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }
	bool GetIsSkeletal() const { return mIsSkeletal; }

protected:
	class Mesh* mMesh;
	bool mIsSkeletal;
};