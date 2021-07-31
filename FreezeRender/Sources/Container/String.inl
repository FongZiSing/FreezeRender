#include "String.hpp"
#ifndef STRING_INL_STRING_IMPL
#define STRING_INL_STRING_IMPL

namespace Detail
{
	namespace StringTraits
	{
		template <typename CharType, bool IsUpper>
		force_inline void Transform(CharType* first, CharType* last)
		{
			const std::ctype<CharType>& ct = std::use_facet<std::ctype<CharType>>(std::locale::classic());
			if constexpr (IsUpper)
			{
				ct.toupper(first, last);
			}
			else
			{
				ct.tolower(first, last);
			}
		}

		template <typename CharType, bool IsCaseSensitive>
		inline bool Compare(const CharType* scope_restrict lhs, const CharType* scope_restrict rhs, const unsigned long long size)
		{
			if constexpr (IsCaseSensitive)
			{
				for (unsigned long long i = 0; i < size; ++i)
				{
					if (lhs[i] != rhs[i])
					{
						return false;
					}
				}
				return true;
			}
			else
			{
				const std::ctype<CharType>& ct = std::use_facet<std::ctype<CharType>>(std::locale::classic());
				for (unsigned long long i = 0; i < size; ++i)
				{
					if (ct.tolower(lhs[i]) != ct.tolower(rhs[i]))
					{
						return false;
					}
				}
				return true;
			}
		}

		template <typename CharType>
		force_inline unsigned int ToUint(const CharType* const scope_restrict target)
		{
			if constexpr (sizeof(CharType) == 1)
			{
				char* endptr;
				return static_cast<unsigned int>(std::strtoul(target, &endptr, 10));
			}
			else if constexpr (sizeof(CharType) == 2)
			{
				wchar_t* endptr;
				return static_cast<unsigned int>(std::wcstoul(target, &endptr, 10));
			}
			else
			{
				static_assert(false, "[FreezeRender] CharType is invalid");
				return 0;
			}
		}
	}
}

#endif // !STRING_INL_STRING_IMPL



#ifndef STRING_INL_STRING_IMPL_STRINGVIEW
#define STRING_INL_STRING_IMPL_STRINGVIEW

	inline StringView::StringView(const String& in) noexcept : str(in.str)
	{
	}

	inline void StringView::operator=(const String& in) noexcept
	{
		this->str = in.str;
	}

	template <typename StringType> requires IsGoodString<StringType>
	force_inline bool StringView::StartWith(const StringType& prefix, const bool bIsCaseSensitive) const noexcept
	{
		const std::string_view& target = prefix.str;
		if (str.size() < target.size() || str.empty() || target.empty())
		{
			return false;
		}

		return bIsCaseSensitive
			? Detail::StringTraits::Compare<char, true>(str.data(), target.data(), target.size())
			: Detail::StringTraits::Compare<char, false>(str.data(), target.data(), target.size());
	}

	force_inline bool StringView::StartWith(const char* const prefix, const bool bIsCaseSensitive) const noexcept
	{
		StringView target = prefix;
		return StartWith(target, bIsCaseSensitive);
	}

	template <typename StringType> requires IsGoodString<StringType>
	force_inline bool StringView::EndWith(const StringType& suffix, const bool bIsCaseSensitive) const noexcept
	{
		const std::string_view& target = suffix.str;
		if (str.size() < target.size() || str.empty() || target.empty())
		{
			return false;
		}

		return bIsCaseSensitive
			? Detail::StringTraits::Compare<char, true>(str.data() + str.size() - target.size(), target.data(), target.size())
			: Detail::StringTraits::Compare<char, false>(str.data() + str.size() - target.size(), target.data(), target.size());
	}

	force_inline bool StringView::EndWith(const char* const suffix, const bool bIsCaseSensitive) const noexcept
	{
		StringView target = suffix;
		return EndWith(target, bIsCaseSensitive);
	}

	force_inline String StringView::ToUpper() const
	{
		String result = *this;
		char* first = &result.str[0];
		char* last = first + result.str.size();
		Detail::StringTraits::Transform<char, true>(first, last);
		return result;
	}
	force_inline String StringView::ToLower() const
	{
		String result = *this;
		char* first = &result.str[0];
		char* last = first + result.str.size();
		Detail::StringTraits::Transform<char, false>(first, last);
		return result;
	}

	template <typename ...StringType> requires IsGoodString<StringType...>
	inline String StringView::Append(const StringType& ...pieces) const
	{
		String result;
		const unsigned long long piecesize = (pieces.str.size() + ...);
		result.str.reserve(this->str.size() + piecesize);
		result.str.append(this->str);
		(result.str.append(pieces.str), ...);
		return result;
	}

	force_inline StringView StringView::Trim() const noexcept
	{
		auto head = str.begin();
		auto tail = str.end() - 1;
		while (head != str.end() && (*head == ' ' || *head == '\t')) head++;
		while (tail != str.begin() && (*tail == ' ' || *tail == '\t')) tail--;


		return StringView(&(*head), tail - head + 1);
	}

	force_inline unsigned int StringView::ToUint() const noexcept
	{
		char* endptr;
		return static_cast<unsigned int>(std::strtoul(str.data(), &endptr, 10));
	}

	force_inline float StringView::ToFloat() const noexcept
	{
		char* endptr;
		return std::strtof(str.data(), &endptr);
	}

#endif // !STRING_INL_STRING_IMPL_STRINGVIEW



#ifndef STRING_INL_STRING_IMPL_STRING
#define STRING_INL_STRING_IMPL_STRING

	template <typename StringType> requires IsGoodString<StringType>
	force_inline bool String::StartWith(const StringType& prefix, const bool bIsCaseSensitive) const noexcept
	{
		const std::string_view& target = prefix.str;
		if (str.size() < target.size() || str.empty() || target.empty())
		{
			return false;
		}

		return bIsCaseSensitive
			? Detail::StringTraits::Compare<char, true>(str.data(), target.data(), target.size())
			: Detail::StringTraits::Compare<char, false>(str.data(), target.data(), target.size());
	}

	force_inline bool String::StartWith(const char* const prefix, const bool bIsCaseSensitive) const noexcept
	{
		StringView target = prefix;
		return StartWith(target, bIsCaseSensitive);
	}

	template <typename StringType> requires IsGoodString<StringType>
	force_inline bool String::EndWith(const StringType& suffix, const bool bIsCaseSensitive) const noexcept
	{
		const std::string_view& target = suffix.str;
		if (str.size() < target.size() || str.empty() || target.empty())
		{
			return false;
		}

		return bIsCaseSensitive
			? Detail::StringTraits::Compare<char, true>(str.data() + str.size() - target.size(), target.data(), target.size())
			: Detail::StringTraits::Compare<char, false>(str.data() + str.size() - target.size(), target.data(), target.size());
	}

	force_inline bool String::EndWith(const char* const suffix, const bool bIsCaseSensitive) const noexcept
	{
		StringView target = suffix;
		return EndWith(target, bIsCaseSensitive);
	}

	force_inline String String::ToUpper() const
	{
		String result = *this;
		char* first = &result.str[0];
		char* last = first + result.str.size();
		Detail::StringTraits::Transform<char, true>(first, last);
		return result;
	}
	force_inline String String::ToLower() const
	{
		String result = *this;
		char* first = &result.str[0];
		char* last = first + result.str.size();
		Detail::StringTraits::Transform<char, false>(first, last);
		return result;
	}

	template<typename ...StringType> requires IsGoodString<StringType...>
	inline String& String::Append(const StringType& ...pieces)
	{
		const unsigned long long size = (pieces.str.size() + ...);
		this->str.reserve(this->str.size() + size);
		(this->str.append(pieces.str), ...);
		return *this;
	}

	force_inline StringView String::Trim() const noexcept
	{
		auto head = str.begin();
		auto tail = str.end() - 1;
		while (head != str.end() && (*head == ' ' || *head == '\t')) head++;
		while (tail != str.begin() && (*tail == ' ' || *tail == '\t')) tail--;


		return StringView(&(*head), tail - head + 1);
	}

	force_inline unsigned int String::ToUint() const noexcept
	{
		char* endptr;
		return static_cast<unsigned int>(std::strtoul(str.data(), &endptr, 10));
	}

	force_inline float String::ToFloat() const noexcept
	{
		char* endptr;
		return std::strtof(str.data(), &endptr);
	}

#endif // !STRING_INL_STRING_IMPL_STRING



#ifndef STRING_INL_STRING_IMPL_WIDESTRINGVIEW
#define STRING_INL_STRING_IMPL_WIDESTRINGVIEW

	inline WideStringView::WideStringView(const WideString& in) noexcept : str(in.str)
	{
	}

	inline void WideStringView::operator=(const WideString& in) noexcept
	{
		this->str = in.str;
	}

	template <typename StringType> requires IsGoodWideString<StringType>
	force_inline bool WideStringView::StartWith(const StringType& prefix, const bool bIsCaseSensitive) const noexcept
	{
		const std::wstring_view& target = prefix.str;
		if (str.size() < target.size() || str.empty() || target.empty())
		{
			return false;
		}

		return bIsCaseSensitive
			? Detail::StringTraits::Compare<wchar_t, true>(str.data(), target.data(), target.size())
			: Detail::StringTraits::Compare<wchar_t, false>(str.data(), target.data(), target.size());
	}

	force_inline bool WideStringView::StartWith(const wchar_t* const prefix, const bool bIsCaseSensitive) const noexcept
	{
		WideStringView target = prefix;
		return StartWith(target, bIsCaseSensitive);
	}

	template <typename StringType> requires IsGoodWideString<StringType>
	force_inline bool WideStringView::EndWith(const StringType& suffix, const bool bIsCaseSensitive) const noexcept
	{
		const std::wstring_view& target = suffix.str;
		if (str.size() < target.size() || str.empty() || target.empty())
		{
			return false;
		}

		return bIsCaseSensitive
			? Detail::StringTraits::Compare<wchar_t, true>(str.data() + str.size() - target.size(), target.data(), target.size())
			: Detail::StringTraits::Compare<wchar_t, false>(str.data() + str.size() - target.size(), target.data(), target.size());
	}

	force_inline bool WideStringView::EndWith(const wchar_t* const suffix, const bool bIsCaseSensitive) const noexcept
	{
		WideStringView target = suffix;
		return EndWith(target, bIsCaseSensitive);
	}

	force_inline WideString WideStringView::ToUpper() const
	{
		WideString result = *this;
		wchar_t* first = &result.str[0];
		wchar_t* last = first + result.str.size();
		Detail::StringTraits::Transform<wchar_t, true>(first, last);
		return result;
	}
	force_inline WideString WideStringView::ToLower() const
	{
		WideString result = *this;
		wchar_t* first = &result.str[0];
		wchar_t* last = first + result.str.size();
		Detail::StringTraits::Transform<wchar_t, false>(first, last);
		return result;
	}

	template <typename ...StringType> requires IsGoodWideString<StringType...>
	inline WideString WideStringView::Append(const StringType& ...pieces) const
	{
		WideString result;
		const unsigned long long piecesize = (pieces.str.size() + ...);
		result.str.reserve(this->str.size() + piecesize);
		result.str.append(this->str);
		(result.str.append(pieces.str), ...);
		return result;
	}

	force_inline WideStringView WideStringView::Trim() const noexcept
	{
		auto head = str.begin();
		auto tail = str.end() - 1;
		while (head != str.end() && (*head == L' ' || *head == L'\t')) head++;
		while (tail != str.begin() && (*tail == L' ' || *tail == L'\t')) tail--;

		return WideStringView(&(*head), tail - head + 1);
	}

	force_inline unsigned int WideStringView::ToUint() const noexcept
	{
		wchar_t* endptr;
		return static_cast<unsigned int>(std::wcstoul(str.data(), &endptr, 10));
	}

	force_inline float WideStringView::ToFloat() const noexcept
	{
		wchar_t* endptr;
		return std::wcstof(str.data(), &endptr);
	}

#endif // !STRING_INL_STRING_IMPL_WIDESTRINGVIEW



#ifndef STRING_INL_STRING_IMPL_WIDESTRING
#define STRING_INL_STRING_IMPL_WIDESTRING

	template <typename StringType> requires IsGoodWideString<StringType>
	force_inline bool WideString::StartWith(const StringType& prefix, const bool bIsCaseSensitive) const noexcept
	{
		const std::wstring_view& target = prefix.str;
		if (str.size() < target.size() || str.empty() || target.empty())
		{
			return false;
		}

		return bIsCaseSensitive
			? Detail::StringTraits::Compare<wchar_t, true>(str.data(), target.data(), target.size())
			: Detail::StringTraits::Compare<wchar_t, false>(str.data(), target.data(), target.size());
	}

	force_inline bool WideString::StartWith(const wchar_t* const prefix, const bool bIsCaseSensitive) const noexcept
	{
		WideStringView target = prefix;
		return StartWith(target, bIsCaseSensitive);
	}

	template <typename StringType> requires IsGoodWideString<StringType>
	force_inline bool WideString::EndWith(const StringType& suffix, const bool bIsCaseSensitive) const noexcept
	{
		const std::wstring_view& target = suffix.str;
		if (str.size() < target.size() || str.empty() || target.empty())
		{
			return false;
		}

		return bIsCaseSensitive
			? Detail::StringTraits::Compare<wchar_t, true>(str.data() + str.size() - target.size(), target.data(), target.size())
			: Detail::StringTraits::Compare<wchar_t, false>(str.data() + str.size() - target.size(), target.data(), target.size());
	}

	force_inline bool WideString::EndWith(const wchar_t* const suffix, const bool bIsCaseSensitive) const noexcept
	{
		WideStringView target = suffix;
		return EndWith(target, bIsCaseSensitive);
	}

	force_inline WideString WideString::ToUpper() const
	{
		WideString result = *this;
		wchar_t* first = &result.str[0];
		wchar_t* last = first + result.str.size();
		Detail::StringTraits::Transform<wchar_t, true>(first, last);
		return result;
	}
	force_inline WideString WideString::ToLower() const
	{
		WideString result = *this;
		wchar_t* first = &result.str[0];
		wchar_t* last = first + result.str.size();
		Detail::StringTraits::Transform<wchar_t, false>(first, last);
		return result;
	}

	template<typename ...StringType> requires IsGoodWideString<StringType...>
	inline WideString& WideString::Append(const StringType& ...pieces)
	{
		const unsigned long long size = (pieces.str.size() + ...);
		this->str.reserve(this->str.size() + size);
		(this->str.append(pieces.str), ...);
		return *this;
	}

	force_inline WideStringView WideString::Trim() const noexcept
	{
		auto head = str.begin();
		auto tail = str.end() - 1;
		while (head != str.end() && (*head == L' ' || *head == L'\t')) head++;
		while (tail != str.begin() && (*tail == L' ' || *tail == L'\t')) tail--;

		return WideStringView(&(*head), tail - head + 1);
	}

	force_inline unsigned int WideString::ToUint() const noexcept
	{
		wchar_t* endptr;
		return static_cast<unsigned int>(std::wcstoul(str.data(), &endptr, 10));
	}

	force_inline float WideString::ToFloat() const noexcept
	{
		wchar_t* endptr;
		return std::wcstof(str.data(), &endptr);
	}

#endif // !STRING_INL_STRING_IMPL_WIDESTRING
