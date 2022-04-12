#pragma once

#include <Common.hpp>
#include <Container/Array.hpp>
#include <Asset/Material/Material.hpp>
#include "Polygon.hpp"



/**
 * @brief The model object.
 */
struct Meshlet
{
	WideString id;
	WideString name;

	Matrix transform = Matrix::Identity;

	Array<Material> materials;
	Array<Vertex> vertices;
	Array<VertexIndex> indices;
	Array<VertexCluster> clusters;


	warn_nodiscard bool IsValid() const
	{
		return !vertices.IsEmpty() && !indices.IsEmpty() && !materials.IsEmpty() && !clusters.IsEmpty();
	}

	unsigned int GetMaterialIndex(const VertexIndex& target)
	{
		for (auto& cluster : clusters)
		{
			if (target.index >= cluster.beginVertexIndex &&
				target.index < cluster.endVertexIndex &&
				cluster.materialIndex < materials.Size())
			{
				return cluster.materialIndex;
			}
		}
		return -1;
	}
};
