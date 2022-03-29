#pragma once
#include <vector>
#include <functional>
#include "Math.h"
#include "Collision.h"

class WorldPhysics {
public:
	WorldPhysics(class Game* game);

	struct CollisionInfo {
		Vector3 cPoint;
		Vector3 cNormal;
		class BoxComponent* cBox;
		class Entity* cEntity;
	};

	void AddBox(class BoxComponent* box);
	void RemoveBox(class BoxComponent* box);

private:
	class Game* gGame;
	std::vector<class BoxComponent*> wBoxes;
};