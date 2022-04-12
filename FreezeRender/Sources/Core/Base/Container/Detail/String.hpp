#pragma once

#include <locale>
#include <algorithm>



namespace StringDetail
{
	template <typename CharType>
	struct StringClassifiedFunctor
	{
	private:
		const std::ctype<CharType>& ctype;
		std::ctype_base::mask mask;

	public:
		StringClassifiedFunctor(std::ctype_base::mask m, std::locale const& loc = std::locale())
			: ctype(std::use_facet< std::ctype<CharType> >(loc))
			, mask(m)
		{}

		bool operator() (const CharType& alphabet) const
		{
			return ctype.is(mask, alphabet);
		}
	};



	template <typename CharType>
	struct StringTransformFunctor
	{
	private:
		const std::ctype<CharType>& ctype;

	public:
		StringTransformFunctor(std::locale const& loc = std::locale())
			: ctype(std::use_facet< std::ctype<CharType> >(loc))
		{}

		template <bool IsUpper>
		bool operator() (const CharType& alphabet) const
		{
			if constexpr (IsUpper)
			{
				return ctype.toupper(alphabet);
			}
			else
			{
				return ctype.tolower(alphabet);
			}
		}

		template <bool IsUpper>
		bool operator() (CharType* first, CharType* last) const
		{
			if constexpr (IsUpper)
			{
				return ctype.toupper(first, last);
			}
			else
			{
				return ctype.tolower(first, last);
			}
		}
	};
}