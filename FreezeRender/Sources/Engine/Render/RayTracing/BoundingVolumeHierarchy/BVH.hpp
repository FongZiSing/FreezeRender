//
// BVH.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Implemention of Bounding Volume Hierarchy.
//
#pragma once

#include <Core/Math/Box.hpp>
#include <Core/Base/Container/Array.hpp>
#include <Asset/Meshlet/Meshlet.hpp>



namespace Pluto
{
	struct BVHNode
	{
		BVHNode* left = nullptr;
		BVHNode* right = nullptr;

		BoundingBox3f bounds;
		bool leaf = false;
		unsigned int first = 0;
		unsigned int count = 0;
	};



	class BVHTree
	{
		Array<BVHNode> origins;
		BVHNode* root;
	public:

		void Build(AMeshlet& meshlet);
	};
}