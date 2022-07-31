#include "BVH.hpp"
#include <Core/Base/Algorithm/TopK.hpp>
#include <queue>


namespace Pluto
{
	struct BVHTriangle
	{
		unsigned int index = 0;
		AVertex *v1, *v2, *v3;
		BoundingBox3f bounds;
		Vector3f centroid;
	};

	struct BVHTriangleCompareX : public BVHTriangle
	{
		constexpr auto operator <=> (const BVHTriangleCompareX& rhs) const
		{
			return centroid.x <=> rhs.centroid.x;
		}

		constexpr auto operator == (const BVHTriangleCompareX& rhs) const
		{
			return centroid.x == rhs.centroid.x;
		}
	};

	struct BVHTriangleCompareY : public BVHTriangle
	{
		constexpr auto operator <=> (const BVHTriangleCompareY& rhs) const
		{
			return centroid.y <=> rhs.centroid.y;
		}

		constexpr auto operator == (const BVHTriangleCompareY& rhs) const
		{
			return centroid.y == rhs.centroid.y;
		}
	};

	struct BVHTriangleCompareZ : public BVHTriangle
	{
		constexpr auto operator <=> (const BVHTriangleCompareZ& rhs) const
		{
			return centroid.z <=> rhs.centroid.z;
		}

		constexpr auto operator == (const BVHTriangleCompareZ& rhs) const
		{
			return centroid.z == rhs.centroid.z;
		}
	};


	void BVHTree::Build(AMeshlet& meshlet)
	{
		const unsigned int numTriangles = (unsigned int)meshlet.indices.Size() / 3;

		Array<BVHTriangle> triangles;
		triangles.Reserve(numTriangles);
		for (unsigned int wedge = 0, *ptr = meshlet.indices.Data(); wedge < numTriangles; ++wedge)
		{
			BVHTriangle& triangle = triangles.AddDefault();
			triangle.index = *ptr;

			triangle.v1 = meshlet.vertices.Data() + *(ptr++);
			triangle.v2 = meshlet.vertices.Data() + *(ptr++);
			triangle.v3 = meshlet.vertices.Data() + *(ptr++);

			triangle.bounds = triangle.v1->position;
			triangle.bounds += triangle.v2->position;
			triangle.bounds += triangle.v3->position;
			triangle.centroid = (triangle.v1->position + triangle.v2->position + triangle.v3->position) * 0.33333333f;
		}

		std::queue<std::pair<int, int>> range;
		range.emplace(0, numTriangles - 1);
		do
		{
			std::pair<int, int> temp = range.front();
			range.pop();
			auto& [left, right] = temp;
			const int num = right - left + 1;
			const int half = num >> 1;
			const int mid = left + half;

			BoundingBox3f bounds;
			for (unsigned int wedge = 0; wedge < numTriangles; ++wedge)
			{
				bounds += triangles[wedge].bounds;
			}
			const auto& [x, y, z] = bounds.GetExtent();

			BVHNode& node = origins.AddDefault();
			node.first = left;
			node.count = num;
			node.bounds = bounds;
			if (num <= 4)
			{
				node.leaf = true;
				continue;
			}

			enum class SplitAxis : unsigned int { X, Y, Z };
			SplitAxis axis = x > y
				? (x > z ? SplitAxis::X : SplitAxis::Z)
				: (y > z ? SplitAxis::Y : SplitAxis::Z);


			if (SplitAxis::X == axis)
			{
				BVHTriangleCompareX* data = static_cast<BVHTriangleCompareX*>(triangles.Data());
				Algorithm::FloydRivestSelect(data + left, num, half);
			}
			else if (SplitAxis::Y == axis)
			{
				BVHTriangleCompareY* data = static_cast<BVHTriangleCompareY*>(triangles.Data());
				Algorithm::FloydRivestSelect(data + left, num, half);
			}
			else
			{
				BVHTriangleCompareZ* data = static_cast<BVHTriangleCompareZ*>(triangles.Data());
				Algorithm::FloydRivestSelect(data + left, num, half);
			}
			range.emplace(left, mid);
			range.emplace(mid + 1, right);

			node.leaf = false;
		} while (!range.empty());
	}
}