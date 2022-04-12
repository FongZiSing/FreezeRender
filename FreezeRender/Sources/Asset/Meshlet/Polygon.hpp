#pragma once

#include <Math/Matrix.hpp>
#include <Math/Color.hpp>



struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector2 uv;
	Color color;
};



struct VertexIndex
{
	unsigned int index = 0;
	Vertex* ptr        = nullptr;
};



struct VertexCluster
{
	unsigned int beginVertexIndex = 0;
	unsigned int endVertexIndex   = 0;
	unsigned int triangleNumber   = 0;
	unsigned int materialIndex    = 0;
};



inline bool operator == (const Vertex& lhs, const Vertex& rhs)
{
	return
		(lhs.position - rhs.position).LengthSquared() < Number::SMALL_NUMBER &&
		(lhs.normal - rhs.normal).LengthSquared() < Number::SMALL_NUMBER &&
		(lhs.uv - rhs.uv).LengthSquared() < Number::SMALL_NUMBER;
}
