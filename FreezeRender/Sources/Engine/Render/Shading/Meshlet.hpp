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
#include "Polygon.hpp"



namespace Pluto
{
	struct Meshlet
	{
	public:
		AMeshlet& data;
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
			const AVertexIndex* begin;
			const AVertexIndex* end;

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
					begin[0].ptr,
					begin[1].ptr,
					begin[2].ptr,
					&mesh.materials[mesh.data.GetMaterialIndex(*begin)]
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