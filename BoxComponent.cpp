#include "BoxComponent.h"
#include "Entity.h"
#include "Game.h"
#include "WorldPhysics.h"

BoxComponent::BoxComponent(Entity* owner, int updateOrder):
	Component(owner, updateOrder),
	objectBox(Vector2::Zero, Vector2::Zero),
	worldBox(Vector2::Zero, Vector2::Zero)
{
	cOwner->GetGame()->GetWorldPhysics()->AddBox(this);
}

BoxComponent::~BoxComponent() {
	cOwner->GetGame()->GetWorldPhysics()->RemoveBox(this);
}

void BoxComponent::OnUpdateWorldTransform() {
	worldBox = objectBox;
	worldBox.aMin *= cOwner->GetScale();
	worldBox.aMax *= cOwner->GetScale();

	worldBox.aMin += Vector2(cOwner->GetPosition().x, cOwner->GetPosition().y);
	worldBox.aMax += Vector2(cOwner->GetPosition().x, cOwner->GetPosition().y);
}