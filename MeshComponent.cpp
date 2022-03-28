#include "MeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Entity.h"
#include "Game.h"
#include "Renderer.h"
#include "VertexArray.h"

MeshComponent::MeshComponent(Entity* owner, bool isSkeletal):
	Component(owner),
	mMesh(nullptr),
	mIsSkeletal(isSkeletal)
{
	cOwner->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent() {
	cOwner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Shader* shader) {
	if (mMesh) {
		shader->SetMatrixUniform("uWorldTransform", cOwner->GetWorldTransform());
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}
