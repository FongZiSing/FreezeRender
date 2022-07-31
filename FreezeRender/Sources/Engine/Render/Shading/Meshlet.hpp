//
// Meshlet.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Implemention of meshlet.
//
#pragma once

#include <Asset/Meshlet/Meshlet.hpp>
#include <Engine/Render/RayTracing/BoundingVolumeHierarchy/BVH.hpp>
#include "Polygon.hpp"



namespace Pluto
{
	struct Meshlet
	{
	public:
		AMeshlet& data;
		BVHTree bvh;
		Array<Material> materials;


		explicit Meshlet(AMeshlet& target)
			: data(target)
		{
			materials.Clear();
			for (auto& perMaterial : target.materials)
			{
				materials.Emplace(perMaterial);
			}
		}

		force_inline bool IsValid() const noexcept
		{
			return data.IsValid();
		}

		class Iterator
		{
		private:
			const Meshlet& mesh;
			const unsigned int* begin;
			const unsigned int* end;

		public:
			explicit Iterator(const Meshlet& target)
				: mesh(target)
				, begin(target.data.indices.Data())
				, end(target.data.indices.Data() + target.data.indices.Size())
			{}

			force_inline void operator++ () { begin += 3; }

			force_inline explicit operator bool() const { return begin < end; }

			force_inline bool operator! () const { return !this->operator bool(); }

			ShadingTriangle Assembly() const
			{
				return ShadingTriangle(
					mesh.data.vertices[begin[0]],
					mesh.data.vertices[begin[1]],
					mesh.data.vertices[begin[2]],
					&mesh.materials[mesh.data.vertices[begin[0]].materialIndex]
				);
			}
		};

		inline Iterator CreateIterator() const
		{
			return Iterator(*this);
		}

		void Build()
		{
			BuildBVH();
		}

	private:
		void BuildBVH();
	};
}