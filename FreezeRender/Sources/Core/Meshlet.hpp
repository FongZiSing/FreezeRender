#pragma once

#include <Common.hpp>
#include <Container/String.hpp>
#include <vector>
#include <memory>
#include "Material.hpp"
#include "Polygon.hpp"



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

	std::vector<Material> materials;

	warn_nodiscard bool IsValid() const
	{
		return !vertices.empty() && !indices.empty();
	}
};
