#pragma once
#include "Math.h"
#include <vector>

struct Circle {
	Circle(const Vector2& center, float radius);

	bool Contains(const Vector2& point) const;

	Vector2 sCenter;
	float sRadius;
};

struct AABB {
	AABB(const Vector2& min, const Vector2& max);

	void UpdateMinMax(const Vector2& point);
	bool Contains(const Vector2& point) const;
	float MinDistSq(const Vector2& point) const;

	Vector2 aMin;
	Vector2 aMax;
};

bool Intersect(const Circle& a, const Circle& b);
bool Intersect(const AABB& a, const AABB& b);
bool Intersect(const Circle& s, const AABB& box);