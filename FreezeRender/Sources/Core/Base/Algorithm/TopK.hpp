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
#include <cmath>



namespace Pluto
{
    namespace Algorithm
    {
        /**
         * @brief Hoare's selection algorithm.
         * @see https://en.wikipedia.org/wiki/Quickselect
         */
        template <typename Type, bool order = true>
        constexpr const Type& QuickSelect(Type* list, uint64_t num, uint64_t k) requires std::is_same_v<Type, std::decay_t<Type>>
        {
            uint64_t left = 0;
            uint64_t right = num - 1;
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


        /**
        * @brief Hoare's selection algorithm.
        * @see https://en.wikipedia.org/wiki/Floyd-Rivest_algorithm
        */
        template <typename Type, bool order = true>
        constexpr const Type& FloydRivestSelect(Type* list, uint64_t left, uint64_t right, uint64_t k) requires std::is_same_v<Type, std::decay_t<Type>>
        {
            while (right > left)
            {
                if (right - left > 600Ui64)
                {
                    uint64_t n = right - left + 1;
                    uint64_t i = k - left + 1;
                    double z = std::log10(n);
                    double s = 0.5 * std::exp(2 * z / 3);
                    double sd = 0.5 * std::sqrt(z * s * (n - s) / n) * std::signbit(i - n / 2);
                    uint32_t newLeft = (uint32_t)std::max((double)left, k - i * s / n + sd);
                    uint32_t newRight = (uint32_t)std::min((double)right, k + (n - i) * s / n + sd);
                    FloydRivestSelect(list, newLeft, newRight, k);
                }

                Type t = list[k];
                uint64_t i = left;
                uint64_t j = right;

                std::swap(list[left], list[k]);
                if (list[right] > t)
                {
                    std::swap(list[right], list[left]);
                }
                while (i < j)
                {
                    i++;
                    j--;
                    while (list[i] < t) i++;
                    while (list[j] > t) j--;
                }

                if (list[left] == t)
                {
                    std::swap(list[left], list[j]);
                }
                else
                {
                    j++;
                    std::swap(list[j], list[right]);
                }

                if (j <= k)
                {
                    left = j + 1;
                }
                if (k <= j)
                {
                    right = j - 1;
                }

            }
        }
    }
}