//
// Algorithm/KahanSum.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Implemention of selection algorithm ( top-k problem ).
//
#pragma once

#include <Common.hpp>
#include <type_traits>



namespace Pluto
{
    namespace Algorithm
    {
        /**
         * @brief Hoare's selection algorithm.
         * @see https://en.wikipedia.org/wiki/Quickselect
         */
        template <typename Type, bool order = true>
        constexpr const Type& QuickSelect(Type* list, uint64_t left, uint64_t right, uint64_t k) requires std::is_same_v<Type, std::decay_t<Type>>
        {
            // Inverse k.
            k = right - left - k;

            while (left != right)
            {
                // Select a pivot index between left right.
                std::size_t index = left + ((right - left + 1) >> 1);

                // The subprocedure called partition.
#pragma region QuickSelect Partition
                {
                    uint64_t start = left;
                    uint64_t cursor = left;
                    const uint64_t& end = right;

                    Type privot = list[index];
                    std::swap(list[index], list[end]);

                    // Maximum top-k.
                    if constexpr (order)
                    {
                        while (start < end && list[start] <= privot)
                        {
                            ++start; ++cursor;
                        }

                        for (std::size_t i = start; i < end; ++i)
                        {
                            if (list[i] <= privot)
                            {
                                std::swap(list[cursor], list[i]);
                                cursor++;
                            }
                        }
                    }
                    // Minimum top-k.
                    else
                    {
                        while (start < end && list[start] >= privot)
                        {
                            ++start; ++cursor;
                        }

                        for (std::size_t i = start; i < end; ++i)
                        {
                            if (list[i] >= privot)
                            {
                                std::swap(list[cursor], list[i]);
                                cursor++;
                            }
                        }
                    }

                    std::swap(list[end], list[cursor]);
                    index = cursor;
                }
#pragma endregion QuickSelect Partition

                if (k == index)
                {
                    return list[k];
                }

                if (k < index)
                {
                    right = index - 1;
                }
                else
                {
                    left = index + 1;
                }
            }

            return list[left];
        }
    }
}