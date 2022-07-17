//
// Container/Array.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Implemention of container for array.
//
#pragma once

#include <Common.hpp>
#include <algorithm>
#include <vector>
#include <array>



namespace Pluto
{
	/**
	 * @brief Templated dynamic array.
	 */
	template <typename ElementType>
	class Array final : private std::vector<ElementType>
	{
		using Super = std::vector<ElementType>;
	public:

		//--------------------------------
		//~ Begin construction.

		using Super::Super;
		constexpr Array(const Array& arr) noexcept : Super(arr) {}
		constexpr Array(Array&& arr) noexcept : Super(std::forward<Array>(arr)) {}
		constexpr Array& operator= (const Array& arr) noexcept { Super::operator=(arr); return *this; }

		constexpr Array(const Super& arr) noexcept : Super(arr) {}
		constexpr Array(Super&& arr) noexcept : Super(std::forward<Super>(arr)) {}
		constexpr Array& operator= (const Super& arr) noexcept { Super::operator=(arr); return *this; }

		//~ End construction.
		//--------------------------------


		//--------------------------------
		//~ Begin range-based iterator.

		using Super::begin;
		using Super::end;
		using Super::operator[];

		//~ End range-based iterator.
		//--------------------------------


		//--------------------------------
		//~ Begin Convert to super.

		constexpr Super& operator* () noexcept { return *this; }
		constexpr const Super& operator*() const noexcept { return *this; }

		//~ End Convert to super.
		//--------------------------------


		constexpr ElementType* Data() noexcept { return Super::data(); }
		constexpr const ElementType* Data() const noexcept { return Super::data(); }
		constexpr Super::size_type Size() const noexcept { return Super::size(); }
		constexpr void Clear() noexcept { Super::clear(); Super::shrink_to_fit(); }
		constexpr void Reserve(Super::size_type capture) noexcept { Super::reserve(capture); }
		constexpr bool IsEmpty() const noexcept { return Super::empty(); }
		constexpr void Swap(Array& arr) noexcept { Super::swap(*arr); }
		constexpr void Swap(Super& arr) noexcept { Super::swap(arr); }
		constexpr void Resize(Super::size_type capture) noexcept { Super::resize(capture); }
		constexpr void Push(ElementType&& element) noexcept { Super::push_back(std::move(element)); }
		constexpr void Push(const ElementType& element) noexcept { Super::push_back(element); }

		constexpr ElementType& AddDefault() noexcept { return Super::emplace_back(); }

		template <typename... Args>
		constexpr auto Emplace(Args&&... args) noexcept -> decltype(Super::emplace_back(std::forward<Args>(args)...))
		{
			return Super::emplace_back(std::forward<Args>(args)...);
		}

		template <typename Predicate>
		inline Array Filter(Predicate&& predicate)
		{
			Array result;
			std::transform(Super::cbegin(), Super::cend(), result.begin(), std::forward<Predicate>(predicate));
			return result;
		}
	};



	/**
	 * @brief Templated static array.
	 */
	template <typename ElementType, size_t ElementSize>
	class StaticArray final
	{
		static_assert(ElementSize > 0, "[FreezeRender] static array can be empty!");

	public:
		using Iterator = std::_Array_iterator<ElementType, ElementSize>;
		using ConstIterator = std::_Array_const_iterator<ElementType, ElementSize>;


		ElementType elements[ElementSize];


		//--------------------------------
		//~ Begin range-based iterator.

		warn_nodiscard constexpr Iterator begin() noexcept { return Iterator(elements, 0); }
		warn_nodiscard constexpr ConstIterator begin() const noexcept { return ConstIterator(elements, 0); }
		warn_nodiscard constexpr Iterator end() noexcept { return Iterator(elements, ElementSize); }
		warn_nodiscard constexpr ConstIterator end() const noexcept { return ConstIterator(elements, ElementSize); }

		warn_nodiscard constexpr ElementType& operator[](size_t pos) noexcept
		{
			assert(pos < ElementSize);
			return elements[pos];
		}

		warn_nodiscard constexpr const ElementType& operator[](size_t pos) const noexcept
		{
			assert(pos < ElementSize);
			return elements[pos];
		}

		//~ End range-based iterator.
		//--------------------------------


		constexpr ElementType* Data() noexcept { elements; }
		constexpr const ElementType* Data() const noexcept { return elements; }
		constexpr size_t Size() const noexcept { return ElementSize; }
		constexpr void Swap(StaticArray& arr) noexcept { std::swap(elements, arr.elements); }
		constexpr void Fill(const ElementType& element) noexcept { std::fill_n(&elements[0], ElementSize, element); }
	};
}