#pragma once

#include <Common.hpp>
#include <Container/String.hpp>
#include <vector>
#include <memory>
#include "Color.hpp"
#include "Texture.hpp"



/**
 * @brief The properties of point in a meshlet.
 */
struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector2 uv;
	Color color;

	force_inline bool operator == (const Vertex& rhs) const
	{
		return (position - rhs.position).LengthSquared() < Number::SMALL_NUMBER &&
			   (normal - rhs.normal).LengthSquared() < Number::SMALL_NUMBER &&
			   (uv - rhs.uv).LengthSquared() < Number::SMALL_NUMBER;
	}
};



/**
 * @brief The index and pointer of point in a meshlet.
 */
struct VertexIndex
{
	unsigned int index = 0;
	Vertex* ptr = nullptr;
};



/**
 * @brief The model object.
 */
class Meshlet
{
public:
	WideString id;
	
	WideString name;

	Matrix transform = Matrix::Identity;

	std::vector<Vertex> vertices;

	std::vector<VertexIndex> indices;

	std::unique_ptr<Texture> texture;

	warn_nodiscard bool IsValid() const
	{
		return !vertices.empty() && !indices.empty();
	}
};
