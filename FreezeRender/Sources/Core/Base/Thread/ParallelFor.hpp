//
// Thread/ParallelFor.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Common parallel operation.
//
#pragma once

#include <Platform/ParallelFor.hpp>



namespace Pluto
{
	/**
	 * @brief Common thread utility. 
	 */
	namespace Thread
	{
		template <typename IndexType, typename Predicate>
		constexpr void ParallelFor(IndexType indexBegin, IndexType indexEnd, Predicate&& perdicate)
		{
			Platform::ParallelFor(indexBegin, indexEnd, std::forward<Predicate>(perdicate));
		}
	}
}