#pragma once

#include <Common.hpp>
#include <Container/Array.hpp>
#include <Asset/Material/Material.hpp>
#include "Polygon.hpp"



/**
 * @brief The model object.
 */
struct AMeshlet
{
	WideString id;
	WideString name;

	Matrix transform = Matrix::Identity;

	Array<AMaterial> materials;
	Array<AVertex> vertices;
	Array<AVertexIndex> indices;
	Array<AVertexCluster> clusters;


	warn_nodiscard bool IsValid() const
	{
		return !vertices.IsEmpty() && !indices.IsEmpty() && !materials.IsEmpty() && !clusters.IsEmpty();
	}

	unsigned int GetMaterialIndex(const AVertexIndex& target)
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
