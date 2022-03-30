#include "WorldPhysics.h"
#include <algorithm>
#include "BoxComponent.h"
#include "Entity.h"
#include <SDL.h>

WorldPhysics::WorldPhysics(Game* game):
	gGame(game)
{}

void WorldPhysics::AddBox(BoxComponent* box) {
	wBoxes.emplace_back(box);
}

void WorldPhysics::RemoveBox(BoxComponent* box) {
	auto iter = std::find(wBoxes.begin(), wBoxes.end(), box);
	if (iter != wBoxes.end()) {
		std::iter_swap(iter, wBoxes.end() - 1);
		wBoxes.pop_back();
	}
}

void WorldPhysics::TestPairwise(std::function<void(Entity*, Entity*)> f) {
	for (size_t i = 0; i < wBoxes.size(); i++) {
		for (size_t j = i + 1; j < wBoxes.size(); j++) {
			BoxComponent* a = wBoxes[i];
			BoxComponent* b = wBoxes[j];
			if (Intersect(a->GetWorldBox(), b->GetWorldBox())) {
				f(a->GetOwner(), b->GetOwner());
			}
		}
	}
}