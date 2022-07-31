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
#include <Core/Math/Math.hpp>
#include <type_traits>



namespace Pluto
{
    namespace Algorithm
    {
        /**
         * @brief Type of top-k problem.
         */
        enum class Selection : unsigned int
        {
            TopK,    // Used to find k-th largest elements.
            BottomK, // Used to find k-th smallest elements.
        };



        /**
         * @brief Hoare's selection algorithm.
         * @see https://en.wikipedia.org/wiki/Quickselect
         */
        template <typename Type>
        constexpr const Type& QuickSelect(Type* list, int64_t num, int64_t k, const Selection& selection = Selection::TopK) requires std::is_same_v<Type, std::decay_t<Type>>
        {
            int64_t left = 0;
            int64_t right = num - 1;

            // Inverse k.
            k = selection == Selection::TopK ? right - left - k : k;

            while (left != right)
            {
                // Select a pivot index between left right.
                int64_t index = left + ((right - left + 1) >> 1);

                // The subprocedure called partition.
#pragma region QuickSelect Partition
                {
                    int64_t start = left;
                    int64_t cursor = left;
                    const int64_t& end = right;

                    Type privot = list[index];
                    std::swap(list[index], list[end]);

                    while (start < end && list[start] <= privot)
                    {
                        ++start; ++cursor;
                    }

                    for (int64_t i = start; i < end; ++i)
                    {
                        if (list[i] <= privot)
                        {
                            std::swap(list[cursor], list[i]);
                            cursor++;
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
        * @brief Floyd-Rivest SELECT algorithm.
        * @see https://en.wikipedia.org/wiki/Floyd-Rivest_algorithm
        * @see https://core.ac.uk/download/pdf/82672439.pdf
        */
        template <typename Type>
        constexpr void FloydRivestSelectInternal(Type* list, int64_t left, int64_t right, int64_t k) requires std::is_same_v<Type, std::decay_t<Type>>
        {
            while (right > left)
            {
                constexpr int64_t arbitraryConstants = 600;
                if (right - left > arbitraryConstants)
                {
                    // Use SELECT recursively on a sample of size `s` to get an estimate
                    // for the `(k-left+1)-th` smallest element int `list[k]`, biased slightly
                    // so that the `(k-left+1)-th` element is expected to lie in the smaller set
                    // after partitioning.
                    int64_t n = right - left + 1;
                    int64_t i = k - left + 1;
                    double z = std::log(n);
                    double s = 0.5 * std::exp(2 * z / 3);
                    double sign = (i - n / 2) >= 0 ? 1 : -1;
                    double sd = 0.5 * std::sqrt(z * s * (n - s) / n) * sign;
                    int64_t newLeft = (int64_t)std::max((double)left, k - i * s / n + sd);
                    int64_t newRight = (int64_t)std::min((double)right, k + (n - i) * s / n + sd);
                    FloydRivestSelectInternal(list, newLeft, newRight, k);
                }

                Type t = list[k];

                // The following code partitions `list[left] ~ list[right]` about `t`.
                // It is similar to partition but will run faster on most machines
                // since subscript range checking on `i` and `j` has been eliminated.
                int64_t i = left;
                int64_t j = right;

                std::swap(list[left], list[k]);
                if (list[right] > t)
                {
                    std::swap(list[right], list[left]);
                }
                while (i < j)
                {
                    std::swap(list[i], list[j]);
                    i++;
                    j--;
                    while (list[i] < t) ++i;
                    while (list[j] > t) --j;
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

                // New adjust `left`, `right` so they surround the subset containing
                // the `(k-left+1)-th` smallest elements.
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

        /**
        * @brief Hoare's selection algorithm.
        * @see https://en.wikipedia.org/wiki/Floyd-Rivest_algorithm
        * @see https://core.ac.uk/download/pdf/82672439.pdf
        */
        template <typename Type, bool order = true>
        constexpr const Type& FloydRivestSelect(Type* list, int64_t num, int64_t k, const Selection& selection = Selection::TopK) requires std::is_same_v<Type, std::decay_t<Type>>
        {
            k = selection == Selection::TopK ? num - 1 - k : k;
            FloydRivestSelectInternal(list, 0, num - 1, k);
            return list[k];
        }
    }
}