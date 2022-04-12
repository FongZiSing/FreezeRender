#pragma once

#include <Asset/Meshlet/Meshlet.hpp>
#include "ShadingPolygon.hpp"



struct ShadingMeshlet
{
	Meshlet& meshlet;
	Array<ShadingMaterial> materials;


	explicit ShadingMeshlet(Meshlet& target)
		: meshlet(target)
	{
		materials.Clear();
		for (auto& perMaterial : target.materials)
		{
			materials.Emplace(perMaterial);
		}
	}

	force_inline bool IsValid() const noexcept
	{
		return meshlet.IsValid();
	}

	class Iterator
	{
		const ShadingMeshlet& entity;
		const VertexIndex* begin;
		const VertexIndex* end;

	public:
		explicit Iterator(const ShadingMeshlet& target)
			: entity(target)
			, begin(target.meshlet.indices.Data())
			, end(target.meshlet.indices.Data() + target.meshlet.indices.Size())
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
				&entity.materials[entity.meshlet.GetMaterialIndex(*begin)]
			);
		}
	};

	inline Iterator CreateIterator() const
	{
		return Iterator(*this);
	}
};
