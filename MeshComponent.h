#pragma once
#include "Component.h"

class MeshComponent : public Component {
public:
	MeshComponent(class Entity* owner);
	~MeshComponent();
	virtual void Draw(class Shader* shader);
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }

protected:
	class Mesh* mMesh;
};