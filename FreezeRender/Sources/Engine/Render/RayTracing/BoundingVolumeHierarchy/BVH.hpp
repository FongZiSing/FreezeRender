//
// BVH.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Implemention of Bounding Volume Hierarchy.
//
#pragma once

#include <Math/Box.hpp>
#include <Container/Array.hpp>
#include <Asset/Meshlet/Meshlet.hpp>



namespace Pluto
{
	struct BVHNode
	{
		BVHNode* left;
		BVHNode* right;

		bool leaf;
		BoundingBox3f bounds;
		unsigned int first;
		unsigned int count;
	};



	class BVHTree
	{
		Array<BVHNode> origins;
		BVHNode* root;
	public:

		void Build(AMeshlet& meshlet);
	};
}