#pragma once

#include <Common.hpp>
#include <type_traits>



/**
 * @brief The unique pointer class of bulk data. 
 */
template<typename Type>
class Bulkdata
{
	// Check type during compilation.
	static_assert(std::is_same_v<Type, std::decay_t<Type>>, "[FreezeRender] only support decay type!");

	// The data pointer.
	Type* ptr;

	// The number of element. 
	unsigned long long count;

public:

	/// Non-copyable.
	Bulkdata(const Bulkdata&) = delete;
	Bulkdata& operator=(const Bulkdata&) = delete;
	/// Non-copyable.

	/// Inline function.
	Bulkdata() noexcept : ptr(nullptr), count(0) {}
	~Bulkdata() { Deallocate(); }
	constexpr explicit operator bool() const noexcept { return ptr != nullptr; }
	constexpr bool operator ! () const noexcept { return ptr == nullptr; }
	constexpr Type& operator [] (const int& index) { return ptr[index]; }
	constexpr const Type& operator [] (const int& index) const { return ptr[index]; }
	constexpr unsigned long long Size() const noexcept { return count; }
	constexpr Type* Get() const noexcept { return ptr; }
	constexpr Type* Get(long long offset) const noexcept { assert((signed)count > offset); return ptr + offset; }
	constexpr void Swap(Bulkdata& rhs) const noexcept { std::swap(ptr, rhs.ptr); std::swap(count, rhs.count); }
	/// Inline function.

	/**
	 * @brief Release old memory, reallocate new one manually.
	 */
	inline void Reallocate(const unsigned long long count);

	/**
	 * @brief Initalize the memory by calling constructor of `Type`.
	 */
	template<typename ...Args>
	inline void Initialize(Args&& ...args);

	/**
	 * @brief Release memory manually.
	 */
	inline void Deallocate();

	/**
	 * @brief Swap memory pointer and count.
	 */
	force_inline void Swap(Bulkdata<Type>& rhs) noexcept;
};



/**
 * @brief The Detail implemention of `Bulkdata` class.
 */
#ifndef BULKDATA_HPP_BULKDATA_IMPL
#define BULKDATA_HPP_BULKDATA_IMPL

	template<typename Type>
	inline void Bulkdata<Type>::Reallocate(const unsigned long long count)
	{
		Deallocate();
		this->ptr = new Type[count];
		this->count = count;
	}

	template<typename Type>
	template<typename ...Args> inline void Bulkdata<Type>::Initialize(Args&& ...args)
	{
		Type temp = { std::forward<Args>(args)... };
		unsigned long long ext = count % 8;
		unsigned long long batch = count / 8;
		Type* begin = ptr;
		while (ext --> 0)
		{
			*(begin++) = temp;
		}

		while (batch --> 0)
		{
			*(begin++) = temp;
			*(begin++) = temp;
			*(begin++) = temp;
			*(begin++) = temp;
			*(begin++) = temp;
			*(begin++) = temp;
			*(begin++) = temp;
			*(begin++) = temp;
		}
	}

	template<typename Type>
	inline void Bulkdata<Type>::Deallocate()
	{
		if (ptr)
		{
			delete[] ptr;
			ptr = nullptr;
		}
		count = 0;
	}

	template<typename Type>
	force_inline void Bulkdata<Type>::Swap(Bulkdata<Type>& rhs) noexcept
	{
		Type* tempPtr = rhs.ptr;
		unsigned long long tempCount = rhs.count;
		rhs.ptr = this->ptr;
		rhs.count = this->count;
		this->ptr = tempPtr;
		this->count = tempCount;
	}

#endif // !BULKDATA_HPP_BULKDATA_IMPL
