//
// Meshlet.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Meshlet asset.
//
#pragma once

#include <Common.hpp>
#include <Container/Array.hpp>
#include <Asset/Material/Material.hpp>
#include "Polygon.hpp"



namespace Pluto
{
	/**
	 * @brief The model object.
	 */
	struct AMeshlet
	{
		WideString id;
		WideString name;

		Matrix44f transform = Matrix44f::Identity;

		Array<AMaterial> materials;
		Array<AVertex> vertices;
		Array<unsigned int> indices;
		Array<AVertexCluster> clusters;


		warn_nodiscard bool IsValid() const
		{
			return !vertices.IsEmpty() && !indices.IsEmpty() && !materials.IsEmpty() && !clusters.IsEmpty();
		}

		unsigned int GetMaterialIndex(const unsigned int& target)
		{
			for (auto& cluster : clusters)
			{
				if (target >= cluster.beginVertexIndex &&
					target < cluster.endVertexIndex &&
					cluster.materialIndex < materials.Size())
				{
					return cluster.materialIndex;
				}
			}
			return -1;
		}
	};
}
