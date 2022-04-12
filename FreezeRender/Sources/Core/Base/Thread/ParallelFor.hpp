#pragma once

#include <ppl.h>



template <typename IndexType, typename Predicate>
constexpr void ParallelFor(IndexType indexBegin, IndexType indexEnd, Predicate&& perdicate)
{
	Concurrency::parallel_for(indexBegin, indexEnd, std::forward<Predicate>(perdicate));
}