#pragma once

#include <Common.hpp>
#include "Detail/String.hpp"



template <typename CharType> class BasicString;
template <typename CharType> class BasicStringView;



template <typename CharType>
class BasicStringView final : private std::basic_string_view<CharType>
{
	using Super = std::basic_string_view<CharType>;

public:
	//--------------------------------
	//~ Begin construction.

	using Super::basic_string_view;
	constexpr BasicStringView(const BasicStringView& v) noexcept : Super(v) {}
	constexpr BasicStringView& operator= (const BasicStringView& v) noexcept { Super::operator=(v); }

	constexpr BasicStringView(const Super& v) noexcept : Super(v) {}
	constexpr BasicStringView& operator= (const Super& v) noexcept { Super::operator=(v); }

	BasicStringView(std::basic_string<CharType>&&) = delete;
	constexpr BasicStringView(const std::basic_string<CharType>& v) noexcept : Super(v) {}
	constexpr BasicStringView& operator= (const std::basic_string<CharType>& v) noexcept { Super::operator=(v); return *this; }

	BasicStringView(BasicString<CharType>&&) = delete;
	inline BasicStringView(const BasicString<CharType>&) noexcept;
	constexpr BasicStringView& operator= (const BasicString<CharType>& v) noexcept { Super::operator=(*v); return *this; }
	
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
	constexpr const Super& operator* () const noexcept { return *this; }

	//~ End Convert to super.
	//--------------------------------


	/** @todo Faster find algorithm instead of stl. */
	template<typename... Args>
	warn_nodiscard constexpr auto Find(Args&&... args) noexcept -> decltype(Super::find(std::forward<Args>(args)...))
	{
		return Super::find(std::forward<Args>(args)...);
	}

	constexpr Super::const_pointer Data() const noexcept { return Super::data(); }
	constexpr Super::size_type Size() const noexcept { return Super::size(); }
	constexpr bool IsEmpty() const noexcept { return Super::empty(); }

	constexpr friend bool operator== (const Super& lhs, const BasicStringView<CharType>& rhs) noexcept
	{
		return rhs._Equal(lhs);
	}

	constexpr friend bool operator== (const std::basic_string<CharType>& lhs, const BasicStringView<CharType>& rhs) noexcept
	{
		return rhs._Equal(lhs);
	}

	constexpr bool operator== (const BasicStringView<CharType>& rhs) const noexcept
	{
		return Super::_Equal(rhs);
	}

	inline bool operator== (const BasicString<CharType>& rhs) const noexcept;

	constexpr bool operator== (const CharType* rhs) const noexcept
	{
		return Super::_Equal(rhs);
	}

	constexpr BasicStringView& TrimLeft(const std::locale& loc = std::locale()) noexcept
	{
		Super::remove_prefix([this, &loc]()
			{
				auto predicate = StringDetail::StringClassifiedFunctor<CharType>(std::ctype_base::space, loc);
				auto first = Super::cbegin();
				auto last = Super::cend();
				for (auto prev = first; prev != last; ++prev)
				{
					if (!predicate(*prev))
						return Super::size_type(prev - first);
				}
				return Super::size_type(0);
			}());
		return *this;
	}

	constexpr BasicStringView& TrimRight(const std::locale& loc = std::locale()) noexcept
	{
		Super::remove_suffix([this, &loc]()
			{
				auto predicate = StringDetail::StringClassifiedFunctor<CharType>(std::ctype_base::space, loc);
				auto first = Super::cbegin();
				auto last = Super::cend();
				for (auto prev = last; prev != first; )
				{
					if (!predicate(*(--prev)))
						return Super::size_type(last - prev - 1);
				}
				return Super::size();
			}());
		return *this;
	}

	constexpr BasicStringView& Trim(const std::locale& loc = std::locale()) noexcept
	{
		TrimRight(loc);
		TrimLeft(loc);
		return *this;
	}

	inline BasicString<CharType> ToUpper(const std::locale& loc = std::locale()) noexcept
	{
		BasicString<CharType> result = *this;
		result.ToUpper();
		return result;
	}

	inline BasicString<CharType> ToLower(const std::locale& loc = std::locale()) noexcept
	{
		BasicString<CharType> result = *this;
		result.ToLower();
		return result;
	}

	/** @todo support wide string. */
	inline unsigned int ToUint() noexcept
	{
		static_assert(std::is_same_v<CharType, char>, "[FreezeRender] support wide string is in todo");
		char* endptr;
		return static_cast<unsigned int>(std::strtoul(Super::data(), &endptr, 10));
	}

	/** @todo support wide string. */
	inline float ToFloat() noexcept
	{
		static_assert(std::is_same_v<CharType, char>, "[FreezeRender] support wide string is in todo");
		char* endptr;
		return std::strtof(Super::data(), &endptr);
	}

	template<typename... Args>
	constexpr bool StartWith(Args&&... args) noexcept
	{
		return Super::size_type(0) == Super::find_first_of(std::forward<Args>(args)...);
	}
};



template<typename CharType>
class BasicString final : private std::basic_string<CharType>
{
	using Super = std::basic_string<CharType>;

public:
	//--------------------------------
	//~ Begin construction.

	using Super::Super;
	constexpr BasicString(const BasicString& v) noexcept : Super(v) {}
	constexpr BasicString(BasicString&& v) noexcept : Super(std::forward<Super>(v)) {}
	constexpr BasicString& operator= (const BasicString& v) noexcept { Super::operator=(v); return *this; }

	constexpr BasicString(const Super& v) noexcept : Super(v) {}
	constexpr BasicString(Super&& v) noexcept : Super(std::forward<Super>(v)) {}
	constexpr BasicString& operator= (const Super& v) noexcept { Super::operator=(v); return *this; }

	constexpr BasicString(const std::basic_string_view<CharType>& v) noexcept : Super(v) {}
	constexpr BasicString& operator= (const std::basic_string_view<CharType>& v) noexcept { Super::operator=(v); return *this; }

	inline BasicString(const BasicStringView<CharType>&);
	constexpr BasicString& operator= (const BasicStringView<CharType>& v) noexcept { Super::operator=(*v); return *this; }

	using Super::operator+=;

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


	/** @todo Faster find algorithm instead of stl. */
	template<typename... Args>
	warn_nodiscard constexpr auto Find(Args&&... args) noexcept -> decltype(Super::find(std::forward<Args>(args)...))
	{
		return Super::find(std::forward<Args>(args)...);
	}

	constexpr CharType* Data() noexcept { return Super::data(); }
	constexpr const CharType* Data() const noexcept { return Super::data(); }
	constexpr Super::size_type Size() const noexcept { return Super::size(); }
	constexpr void Clear() noexcept { Super::clear(); Super::shrink_to_fit(); }
	constexpr bool IsEmpty() const noexcept { return Super::empty(); }
	constexpr void Reserve(Super::size_type capture) { Super::reserve(capture); }

	constexpr friend bool operator== (const Super& lhs, const BasicString<CharType>& rhs) noexcept
	{
		return lhs._Equal(*rhs);
	}

	constexpr friend bool operator== (const std::basic_string_view<CharType>& lhs, const BasicString<CharType>& rhs) noexcept
	{
		return lhs._Equal(*rhs);
	}

	constexpr bool operator== (const BasicString<CharType>& rhs) const noexcept
	{
		return Super::_Equal(rhs);
	}

	inline bool operator== (const BasicStringView<CharType>& rhs) const noexcept;

	constexpr bool operator== (const CharType* rhs) const noexcept
	{
		return Super::_Equal(rhs);
	}

	constexpr BasicString& TrimLeft(const std::locale& loc = std::locale()) noexcept
	{
		Super::erase(Super::cbegin(), [this, &loc]()
			{
				auto predicate = StringDetail::StringClassifiedFunctor<CharType>(std::ctype_base::space, loc);;
				auto first = Super::cbegin();
				auto last = Super::cend();
				for (; first != last; ++first)
				{
					if (!predicate(*first))
						return first;
				}
				return first;
			}());
		return *this;
	}

	constexpr BasicString& TrimRight(const std::locale& loc = std::locale()) noexcept
	{
		Super::erase([this, &loc]()
			{
				auto predicate = StringDetail::StringClassifiedFunctor<CharType>(std::ctype_base::space, loc);;
				auto first = Super::cbegin();
				auto last = Super::cend();
				for (; last != first; )
				{
					if (!predicate(*(--last)))
						return ++last;
				}
				return first;
			}(), Super::cend());
		return *this;
	}

	constexpr BasicString& Trim(const std::locale& loc = std::locale()) noexcept
	{
		TrimRight(loc);
		TrimLeft(loc);
		return *this;
	}

	inline BasicStringView<CharType> TrimView(const std::locale& loc = std::locale()) noexcept
	{
		return BasicStringView<CharType>(*this).Trim(loc);
	}

	template<typename... Args>
	warn_nodiscard constexpr auto Append(Args&&... args) -> decltype(Super::append(std::forward<Args>(args)...))
	{
		return Super::append(std::forward<Args>(args)...);
	}

	inline BasicString& ToUpper(const std::locale& loc = std::locale()) noexcept
	{
		auto predicate = StringDetail::StringTransformFunctor<CharType>(loc);
		predicate<true>( &(*Super::begin()), &(*Super::end()) );
		return *this;
	}

	inline BasicString& ToLower(const std::locale& loc = std::locale()) noexcept
	{
		auto predicate = StringDetail::StringTransformFunctor<CharType>(loc);
		predicate<false>( &(*Super::begin()), &(*Super::end()) );
		return *this;
	}

	/** @todo support wide string. */
	inline unsigned int ToUint() noexcept
	{
		static_assert(std::is_same_v<CharType, char>, "[FreezeRender] support wide string is in todo");
		char* endptr;
		return static_cast<unsigned int>(std::strtoul(Super::data(), &endptr, 10));
	}

	/** @todo support wide string. */
	inline float ToFloat() noexcept
	{
		static_assert(std::is_same_v<CharType, char>, "[FreezeRender] support wide string is in todo");
		char* endptr;
		return std::strtof(Super::data(), &endptr);
	}
};



template <typename CharType> inline BasicStringView<CharType>::BasicStringView(const BasicString<CharType>& s) noexcept : std::string_view(*s) {}
template <typename CharType> inline BasicString<CharType>::BasicString(const BasicStringView<CharType>& sv) : std::string(*sv) {}
template <typename CharType> inline bool BasicStringView<CharType>::operator== (const BasicString<CharType>& rhs) const noexcept { return *rhs == *this; }
template <typename CharType> inline bool BasicString<CharType>::operator== (const BasicStringView<CharType>& rhs) const noexcept { return *rhs == *this; }



using String = BasicString<char>;
using StringView = BasicStringView<char>;
using WideString = BasicString<wchar_t>;
using WideStringView = BasicStringView<wchar_t>;