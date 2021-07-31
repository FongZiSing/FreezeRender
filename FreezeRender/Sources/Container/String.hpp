#pragma once

#include <Common.hpp>
#include <string>
#include <locale>

/// forward declaration.
class String;
class StringView;
class WideString;
class WideStringView;
/// forward declaration.



/**
 * The concept to determine whether is `String` or `StringView` class, used for their member functions.
 */
template <typename ...StringType>
concept IsGoodString = ((
	(std::is_same_v<std::decay_t<StringType>, String> || std::is_same_v<std::decay_t<StringType>, StringView>) && // Is `String` or `StringView` class.
	!std::is_pointer_v<StringType> // Not a pointer.
) && ...);



/**
 * The concept to determine whether is `WideString` or `WideStringView` class, used for their member functions.
 */
template <typename ...StringType>
concept IsGoodWideString = ((
	(std::is_same_v<std::decay_t<StringType>, WideString> || std::is_same_v<std::decay_t<StringType>, WideStringView>) && // Is `String` or `StringView` class.
	!std::is_pointer_v<StringType> // Not a pointer.
	) && ...);



/**
 * @brief A warpper class of std::string_view.
 *        std::string_view is just designed as an array of characters without regard to character encoding or sensitive,
 *        and, some useful string operations has not supported, such as Upper(), Lower().
 *        Maybe reimplement a string_view class in the future when necessary.
 */
class StringView final
{
	friend class String;
	std::string_view str;

public:
	/// Initialize.
	inline StringView() noexcept : str() {}
	inline StringView(const String& in) noexcept;
	inline StringView(const StringView& in) noexcept : str(in.str) {}
	inline StringView(const std::string& in) noexcept : str(in) {}
	inline StringView(const std::string_view& in) noexcept : str(in) {}
	inline StringView(const char* data) noexcept : str(data) {}
	inline StringView(const char* data, const unsigned long long count) noexcept : str(data, count) {}
	inline StringView(const char* begin, const char* end) noexcept : str(begin, end) {}

	inline void operator = (const String& in) noexcept;
	inline void operator = (const StringView& in) noexcept { this->str = in.str; }
	inline void operator = (const std::string& in) noexcept { this->str = in; }
	inline void operator = (const std::string_view& in) noexcept { this->str = in; }
	/// Initialize.

	/// Inline function.
	constexpr const std::string_view& Native() const noexcept { return str; }
	constexpr const char* Data() noexcept { return str.data(); }
	constexpr int Size() noexcept { return static_cast<int>(str.length()); }
	constexpr bool IsEmpty() noexcept { return str.empty(); }
	constexpr const char& Front() const noexcept { return str.front(); }
	constexpr const char& Back() const noexcept { return str.back(); }
	constexpr const char& operator [](const int& index) const noexcept { return str[index]; }
	template <typename StringType> requires IsGoodString<StringType> constexpr bool operator == (const StringType& rhs) { return str == rhs.str; }
	template <typename StringType> constexpr bool operator == (const StringType& rhs) { return str == rhs; }
	/// Inline function.

	/**
	 * @brief Determines whether this string starts with given string.
	 * @tparam StringType         - `String` or `StringView` class.
	 * @param prefix              - The target string for searching.
	 * @param bIsCaseSensitive    - Indicates whether the search is case sensitive or not.
	 */
	template <typename StringType> requires IsGoodString<StringType>
	force_inline bool StartWith(const StringType& prefix, const bool bIsCaseSensitive = true) const noexcept;

	/**
	 * @brief Determines whether this string starts with given string.
	 * @param prefix              - The target string for searching.
	 * @param bIsCaseSensitive    - Indicates whether the search is case sensitive or not.
	 */
	force_inline bool StartWith(const char* const prefix, const bool bIsCaseSensitive = true) const noexcept;

	/**
	 * @brief Determines whether this string ends with given string.
	 * @tparam StringType         - `String` or `StringView` class.
	 * @param suffix              - The target string for searching.
	 * @param bIsCaseSensitive    - Indicates whether the search is case sensitive or not.
	 */
	template <typename StringType> requires IsGoodString<StringType>
	force_inline bool EndWith(const StringType& suffix, const bool bIsCaseSensitive = true) const noexcept;

	/**
	 * @brief Determines whether this string ends with given string.
	 * @param suffix              - The target string for searching.
	 * @param bIsCaseSensitive    - Indicates whether the search is case sensitive or not.
	 */
	force_inline bool EndWith(const char* const suffix, const bool bIsCaseSensitive = true) const noexcept;

	/**
	 * @brief Returns a new string with the characters of this converted to uppercase.
	 */
	warn_nodiscard force_inline String ToUpper() const;

	/**
	 * @brief Returns a new string with the characters of this converted to lowercase.
	 */
	warn_nodiscard force_inline String ToLower() const;

	/**
	 * @brief Append a set of string and return a new string.
	 * @tparam StringType    - `String` or `StringView` class.
	 * @param pieces         - A set of string.
	 */
	template <typename ...StringType> requires IsGoodString<StringType...>
	warn_nodiscard inline String Append(const StringType& ...pieces) const;

	/**
	 * @brief Removes `\whitespace` and `\tab` from the start and end of this string.
	 *        The source string is not modified, and just returns a stringview to result.
	 */
	warn_nodiscard force_inline StringView Trim() const noexcept;

	/**
	 * @brief Convert this string to 32-bit unsigned integer.
	 */
	warn_nodiscard force_inline unsigned int ToUint() const noexcept;

	/**
	 * @brief Convert this string to 32-bit float number.
	 */
	warn_nodiscard force_inline float ToFloat() const noexcept;
};



/**
 * @brief A warpper class of std::string.
 *        std::string is just designed as an array of characters without regard to character encoding or sensitive,
 *        and some useful string operations has not supported, such as Upper(), Lower().
 *        Maybe reimplement a string class in the future when necessary.
 */
class String final
{
	friend class StringView;
	std::string str;
public:

	/// Initialize.
	inline String() noexcept : str() {}
	inline String(const String& in) noexcept : str(in.str) {}
	inline String(String&& in) noexcept : str(std::move(in.str)) {}
	inline String(const StringView& in) noexcept : str(in.str) {}
	inline String(const std::string& in) noexcept : str(in) {}
	inline String(std::string&& in) noexcept : str(std::move(in)) {}
	inline String(const std::string_view& in) noexcept : str(in) {}
	inline String(const char* data) noexcept : str(data) {}
	inline String(const char* data, const unsigned long long count) noexcept : str(data, count) {}
	inline String(const char* begin, const char* end) noexcept : str(begin, end) {}

	inline void operator = (const String& in) noexcept { this->str = in.str; }
	inline void operator = (String&& in) noexcept { this->str = std::move(in.str); }
	inline void operator = (const StringView& in) noexcept { this->str = in.str; }
	inline void operator = (const std::string& in) noexcept { this->str = in; }
	inline void operator = (std::string&& in) noexcept { this->str = std::move(in); }
	inline void operator = (const std::string_view& in) noexcept { this->str = in; }
	/// Initialize.

	/// Inline function.
	constexpr std::string& Native() noexcept { return str; }
	constexpr char* Data() noexcept { return str.data(); }
	constexpr int Size() noexcept { return static_cast<int>(str.length()); }
	constexpr void Clear() noexcept { str.clear(); }
	constexpr bool IsEmpty() noexcept { return str.empty(); }
	constexpr char& Front() noexcept { return str.front(); }
	constexpr char& Back() noexcept { return str.back(); }
	constexpr const char& operator [](const int& index) const noexcept { return str[index]; }
	constexpr char& operator [](const int& index) noexcept { return str[index]; }
	template <typename StringType> requires IsGoodString<StringType> constexpr bool operator == (const StringType& rhs) { return str == rhs.str; }
	template <typename StringType> constexpr bool operator == (const StringType& rhs) { return str == rhs; }
	/// Inline function.

	/**
	 * @brief Determines whether this string starts with given string.
	 * @tparam StringType         - `String` or `StringView` class.
	 * @param prefix              - The target string for searching.
	 * @param bIsCaseSensitive    - Indicates whether the search is case sensitive or not.
	 */
	template <typename StringType> requires IsGoodString<StringType>
	force_inline bool StartWith(const StringType& prefix, const bool bIsCaseSensitive = true) const noexcept;

	/**
	 * @brief Determines whether this string starts with given string.
	 * @param prefix              - The target string for searching.
	 * @param bIsCaseSensitive    - Indicates whether the search is case sensitive or not.
	 */
	force_inline bool StartWith(const char* const prefix, const bool bIsCaseSensitive = true) const noexcept;

	/**
	 * @brief Determines whether this string ends with given string.
	 * @tparam StringType         - `String` or `StringView` class.
	 * @param suffix              - The target string for searching.
	 * @param bIsCaseSensitive    - Indicates whether the search is case sensitive or not.
	 */
	template <typename StringType> requires IsGoodString<StringType>
	force_inline bool EndWith(const StringType& suffix, const bool bIsCaseSensitive = true) const noexcept;

	/**
	 * @brief Determines whether this string ends with given string.
	 * @param suffix              - The target string for searching.
	 * @param bIsCaseSensitive    - Indicates whether the search is case sensitive or not.
	 */
	force_inline bool EndWith(const char* const suffix, const bool bIsCaseSensitive = true) const noexcept;

	/**
	 * @brief Returns a new string with the characters of this converted to uppercase.
	 */
	warn_nodiscard force_inline String ToUpper() const;

	/**
	 * @brief Returns a new string with the characters of this converted to lowercase.
	 */
	warn_nodiscard force_inline String ToLower() const;

	/**
	 * @brief Append a set of string and return a reference to this.
	 * @tparam StringType    - `String` or `StringView` class.
	 * @param pieces         - A set of string.
	 */
	template <typename ...StringType> requires IsGoodString<StringType...>
	inline String& Append(const StringType& ...pieces);

	/**
	 * @brief Removes `\whitespace` and `\tab` from the start and end of this string.
	 *        The source string is not modified, and just returns a stringview to result.
	 */
	warn_nodiscard force_inline StringView Trim() const noexcept;

	/**
	 * @brief Convert this string to 32-bit unsigned integer.
	 */
	warn_nodiscard force_inline unsigned int ToUint() const noexcept;

	/**
	 * @brief Convert this string to 32-bit float number.
	 */
	warn_nodiscard force_inline float ToFloat() const noexcept;
};



/**
 * @brief A warpper class of std::wstring_view.
 *        std::string_view is just designed as an array of characters without regard to character encoding or sensitive,
 *        and, some useful string operations has not supported, such as Upper(), Lower().
 *        Maybe reimplement a wstring_view class in the future when necessary.
 */
class WideStringView final
{
	friend class WideString;
	std::wstring_view str;

public:
	/// Initialize.
	inline WideStringView() noexcept : str() {}
	inline WideStringView(const WideString& in) noexcept;
	inline WideStringView(const WideStringView& in) noexcept : str(in.str) {}
	inline WideStringView(const std::wstring& in) noexcept : str(in) {}
	inline WideStringView(const std::wstring_view& in) noexcept : str(in) {}
	inline WideStringView(const wchar_t* data) noexcept : str(data) {}
	inline WideStringView(const wchar_t* data, const unsigned long long count) noexcept : str(data, count) {}
	inline WideStringView(const wchar_t* begin, const wchar_t* end) noexcept : str(begin, end) {}

	inline void operator = (const WideString& in) noexcept;
	inline void operator = (const WideStringView& in) noexcept { this->str = in.str; }
	inline void operator = (const std::wstring& in) noexcept { this->str = in; }
	inline void operator = (const std::wstring_view& in) noexcept { this->str = in; }
	/// Initialize.

	/// Inline function.
	constexpr const std::wstring_view& Native() const noexcept { return str; }
	constexpr const wchar_t* Data() noexcept { return str.data(); }
	constexpr int Size() noexcept { return static_cast<int>(str.length()); }
	constexpr bool IsEmpty() noexcept { return str.empty(); }
	constexpr const wchar_t& Front() const noexcept { return str.front(); }
	constexpr const wchar_t& Back() const noexcept { return str.back(); }
	constexpr const wchar_t& operator [](const int& index) const noexcept { return str[index]; }
	template <typename StringType> requires IsGoodWideString<StringType> constexpr bool operator == (const StringType& rhs) { return str == rhs.str; }
	template <typename StringType> constexpr bool operator == (const StringType& rhs) { return str == rhs; }
	/// Inline function.

	/**
	 * @brief Determines whether this string starts with given string.
	 * @tparam StringType         - `String` or `StringView` class.
	 * @param prefix              - The target string for searching.
	 * @param bIsCaseSensitive    - Indicates whether the search is case sensitive or not.
	 */
	template <typename StringType> requires IsGoodWideString<StringType>
	force_inline bool StartWith(const StringType& prefix, const bool bIsCaseSensitive = true) const noexcept;
	
	/**
	 * @brief Determines whether this string starts with given string.
	 * @param prefix              - The target string for searching.
	 * @param bIsCaseSensitive    - Indicates whether the search is case sensitive or not.
	 */
	force_inline bool StartWith(const wchar_t* const prefix, const bool bIsCaseSensitive = true) const noexcept;

	/**
	 * @brief Determines whether this string ends with given string.
	 * @tparam StringType         - `String` or `StringView` class.
	 * @param suffix              - The target string for searching.
	 * @param bIsCaseSensitive    - Indicates whether the search is case sensitive or not.
	 */
	template <typename StringType> requires IsGoodWideString<StringType>
	force_inline bool EndWith(const StringType& suffix, const bool bIsCaseSensitive = true) const noexcept;

	/**
	 * @brief Determines whether this string ends with given string.
	 * @param suffix              - The target string for searching.
	 * @param bIsCaseSensitive    - Indicates whether the search is case sensitive or not.
	 */
	force_inline bool EndWith(const wchar_t* const suffix, const bool bIsCaseSensitive = true) const noexcept;

	/**
	 * @brief Returns a new string with the characters of this converted to uppercase.
	 */
	warn_nodiscard force_inline WideString ToUpper() const;

	/**
	 * @brief Returns a new string with the characters of this converted to lowercase.
	 */
	warn_nodiscard force_inline WideString ToLower() const;

	/**
	 * @brief Append a set of string and return a new string.
	 * @tparam StringType    - `String` or `StringView` class.
	 * @param pieces         - A set of string.
	 */
	template <typename ...StringType> requires IsGoodWideString<StringType...>
	warn_nodiscard inline WideString Append(const StringType& ...pieces) const;

	/**
	 * @brief Removes `\whitespace` and `\tab` from the start and end of this string.
	 *        The source string is not modified, and just returns a stringview to result.
	 */
	warn_nodiscard force_inline WideStringView Trim() const noexcept;

	/**
	 * @brief Convert this string to 32-bit unsigned integer.
	 */
	warn_nodiscard force_inline unsigned int ToUint() const noexcept;

	/**
	 * @brief Convert this string to 32-bit float number.
	 */
	warn_nodiscard force_inline float ToFloat() const noexcept;
};



/**
 * @brief A warpper class of std::wstring.
 *        std::string is just designed as an array of characters without regard to character encoding or sensitive,
 *        and some useful string operations has not supported, such as Upper(), Lower().
 *        Maybe reimplement a wstring class in the future when necessary.
 */
class WideString final
{
	friend class WideStringView;
	std::wstring str;
public:

	/// Initialize.
	inline WideString() noexcept : str() {}
	inline WideString(const WideString& in) noexcept : str(in.str) {}
	inline WideString(WideString&& in) noexcept : str(std::move(in.str)) {}
	inline WideString(const WideStringView& in) noexcept : str(in.str) {}
	inline WideString(const std::wstring& in) noexcept : str(in) {}
	inline WideString(std::wstring&& in) noexcept : str(std::move(in)) {}
	inline WideString(const std::wstring_view& in) noexcept : str(in) {}
	inline WideString(const wchar_t* data) noexcept : str(data) {}
	inline WideString(const wchar_t* data, const unsigned long long count) noexcept : str(data, count) {}
	inline WideString(const wchar_t* begin, const wchar_t* end) noexcept : str(begin, end) {}

	inline void operator = (const WideString& in) noexcept { this->str = in.str; }
	inline void operator = (WideString&& in) noexcept { this->str = std::move(in.str); }
	inline void operator = (const WideStringView& in) noexcept { this->str = in.str; }
	inline void operator = (const std::wstring& in) noexcept { this->str = in; }
	inline void operator = (std::wstring&& in) noexcept { this->str = std::move(in); }
	inline void operator = (const std::wstring_view& in) noexcept { this->str = in; }
	/// Initialize.

	/// Inline function.
	constexpr std::wstring& Native() noexcept { return str; }
	constexpr wchar_t* Data() noexcept { return str.data(); }
	constexpr int Size() noexcept { return static_cast<int>(str.length()); }
	constexpr void Clear() noexcept { str.clear(); }
	constexpr bool IsEmpty() noexcept { return str.empty(); }
	constexpr wchar_t& Front() noexcept { return str.front(); }
	constexpr wchar_t& Back() noexcept { return str.back(); }
	constexpr const wchar_t& operator [](const int& index) const noexcept { return str[index]; }
	constexpr wchar_t& operator [](const int& index) noexcept { return str[index]; }
	template <typename StringType> requires IsGoodWideString<StringType> constexpr bool operator == (const StringType& rhs) { return str == rhs.str; }
	template <typename StringType> constexpr bool operator == (const StringType& rhs) { return str == rhs; }
	/// Inline function.

	/**
	 * @brief Determines whether this string starts with given string.
	 * @tparam StringType         - `String` or `StringView` class.
	 * @param prefix              - The target string for searching.
	 * @param bIsCaseSensitive    - Indicates whether the search is case sensitive or not.
	 */
	template <typename StringType> requires IsGoodWideString<StringType>
	force_inline bool StartWith(const StringType& prefix, const bool bIsCaseSensitive = true) const noexcept;
	
	/**
	 * @brief Determines whether this string starts with given string.
	 * @param prefix              - The target string for searching.
	 * @param bIsCaseSensitive    - Indicates whether the search is case sensitive or not.
	 */
	force_inline bool StartWith(const wchar_t* const prefix, const bool bIsCaseSensitive = true) const noexcept;

	/**
	 * @brief Determines whether this string ends with given string.
	 * @tparam StringType         - `String` or `StringView` class.
	 * @param suffix              - The target string for searching.
	 * @param bIsCaseSensitive    - Indicates whether the search is case sensitive or not.
	 */
	template <typename StringType> requires IsGoodWideString<StringType>
	force_inline bool EndWith(const StringType& suffix, const bool bIsCaseSensitive = true) const noexcept;

	/**
	 * @brief Determines whether this string ends with given string.
	 * @param suffix              - The target string for searching.
	 * @param bIsCaseSensitive    - Indicates whether the search is case sensitive or not.
	 */
	force_inline bool EndWith(const wchar_t* const suffix, const bool bIsCaseSensitive = true) const noexcept;

	/**
	 * @brief Returns a new string with the characters of this converted to uppercase.
	 */
	warn_nodiscard force_inline WideString ToUpper() const;

	/**
	 * @brief Returns a new string with the characters of this converted to lowercase.
	 */
	warn_nodiscard force_inline WideString ToLower() const;

	/**
	 * @brief Append a set of string and return a reference to this.
	 * @tparam StringType    - `String` or `StringView` class.
	 * @param pieces         - A set of string.
	 */
	template <typename ...StringType> requires IsGoodWideString<StringType...>
	inline WideString& Append(const StringType& ...pieces);

	/**
	 * @brief Removes `\whitespace` and `\tab` from the start and end of this string.
	 *        The source string is not modified, and just returns a stringview to result.
	 */
	warn_nodiscard force_inline WideStringView Trim() const noexcept;

	/**
	 * @brief Convert this string to 32-bit unsigned integer.
	 */
	warn_nodiscard force_inline unsigned int ToUint() const noexcept;

	/**
	 * @brief Convert this string to 32-bit float number.
	 */
	warn_nodiscard force_inline float ToFloat() const noexcept;
};



/**
 * @brief The Detail implemention of `String` and `StringView` class.
 */
#include "String.inl"