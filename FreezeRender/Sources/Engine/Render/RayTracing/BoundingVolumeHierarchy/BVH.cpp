#include "BVH.hpp"
#include <algorithm>


namespace Pluto
{
	struct BVHTriangle
	{
		unsigned int index;
		BoundingBox3f bounds;
		Vector3f centroid;
	};


	void BVHTree::Build(AMeshlet& meshlet)
	{
		const unsigned int numTriangles = (unsigned int)meshlet.indices.Size() / 3;

		Array<BVHTriangle> triangles;
		triangles.Reserve(numTriangles);
		for (unsigned int wedge = 0, *ptr = meshlet.indices.Data(); wedge < numTriangles; wedge += 3)
		{
			BVHTriangle& triangle = triangles.AddDefault();
			triangle.index = *ptr;

			const AVertex& v1 = meshlet.vertices[*(ptr++)];
			const AVertex& v2 = meshlet.vertices[*(ptr++)];
			const AVertex& v3 = meshlet.vertices[*(ptr++)];

			triangle.bounds = v1.position;
			triangle.bounds += v2.position;
			triangle.bounds += v3.position;
			triangle.centroid = (v1.position + v2.position + v3.position) * 0.33333333f;
		}

		BoundingBox3f bounds = triangles[0].bounds;
		for (unsigned int wedge = 1; wedge < numTriangles; ++wedge)
		{
			bounds += triangles[wedge].bounds;
		}

		do
		{
			Vector3f extent = bounds.GetExtent();
			const auto& [x, y, z] = extent;

			enum class SplitAxis : unsigned int { X, Y, Z };
			SplitAxis axis = x > y
				? (x > z ? SplitAxis::X : SplitAxis::Z)
				: (y > z ? SplitAxis::Y : SplitAxis::Z);

			if (SplitAxis::X == axis)
			{
				// TODO
			}


		} while (false);
	}
}