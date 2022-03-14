#pragma once
#include <vector>
#include <string>

class Mesh {
public:
	Mesh();
	~Mesh();
	bool Load(const std::string& fileName, class Renderer* renderer);
	void Unload();
	class VertexArray* GetVertexArray() { return mVertexArray; }
	const std::string& GetShaderName() const { return mShaderName; }
	float GetRadius() const { return mRadius; }
	float GetSpecPower() const { return mSpecPower; }

private:
	class VertexArray* mVertexArray;
	std::string mShaderName;
	float mRadius;
	float mSpecPower;
};