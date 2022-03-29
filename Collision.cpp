#include "Collision.h"
#include <algorithm>
#include <array>

//Collision Objects
Circle::Circle(const Vector2& center, float radius) :
	sCenter(center),
	sRadius(radius)
{}

AABB::AABB(const Vector2& min, const Vector2& max) :
	aMin(min),
	aMax(max)
{}


//Collision Object Functions
bool Circle::Contains(const Vector2& point) const {
	float distSq = (sCenter - point).LengthSq();
	return distSq <= (sRadius * sRadius);
}


void AABB::UpdateMinMax(const Vector2& point) {
	aMin.x = Math::Min(aMin.x, point.x);
	aMin.y = Math::Min(aMin.y, point.y);

	aMax.x = Math::Max(aMax.x, point.x);
	aMax.y = Math::Max(aMax.y, point.y);
}

bool AABB::Contains(const Vector2& point) const {
	return !(point.x < aMin.x || point.y < aMin.y || point.x > aMax.x || point.y > aMax.y);
}

float AABB::MinDistSq(const Vector2& point) const {
	float dx = Math::Max(aMin.x - point.x, 0.0f);
	dx = Math::Max(dx, point.x - aMax.x);
	float dy = Math::Max(aMin.y - point.y, 0.0f);
	dy = Math::Max(dy, point.y - aMax.y);

	return dx * dx + dy * dy;
}


//Collision Detection Functions
bool Intersect(const Circle& a, const Circle& b) {
	float distSq = (a.sCenter - b.sCenter).LengthSq();
	float sumRadii = a.sRadius + b.sRadius;
	return distSq <= (sumRadii * sumRadii);
}

bool Intersect(const AABB& a, const AABB& b) {
	return  !(a.aMax.x < b.aMin.x || a.aMax.y < b.aMin.y || b.aMax.x < a.aMin.x || b.aMax.y < a.aMin.y);
}

bool Intersect(const Circle& s, const AABB& box) {
	float distSq = box.MinDistSq(s.sCenter);
	return distSq <= (s.sRadius * s.sRadius);
}