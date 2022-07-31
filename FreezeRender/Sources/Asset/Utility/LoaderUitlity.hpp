//
// LoaderUitlity.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Utility sets for loader.
//
#pragma once

#include <Core/Base/Container/String.hpp>
#include <Core/Base/Container/Array.hpp>
#include <filesystem>



namespace Pluto
{
	/**
	 * @brief The descriptor of file extesion.
	*/
	struct ExtesionDescriptor
	{
		const wchar_t* target;
		const unsigned char reserved1;
	};



	/**
	 * @brief Determines whether file extesion is equals to the specified one.
	*/
	inline bool IsExtesionEqual(const std::filesystem::path& filepath, const wchar_t* target)
	{
		auto ext = filepath.extension().native();

		// Converts the given string to lowercase.
		const std::ctype<wchar_t>& ct = std::use_facet<std::ctype<wchar_t> >(std::locale::classic());
		wchar_t* first = &ext[0];
		wchar_t* last = first + ext.size();
		ct.tolower(first, last);

		return ext == target;
	}



	/**
	 * @brief Determines whether file extesion is equals to the one of specified targets.
	*/
	inline int IsExtesionEqual(const std::filesystem::path& filepath, const Array<ExtesionDescriptor>& targets)
	{
		auto ext = filepath.extension().native();

		// Converts the given string to lowercase.
		const std::ctype<wchar_t>& ct = std::use_facet<std::ctype<wchar_t> >(std::locale::classic());
		wchar_t* first = &ext[0];
		wchar_t* last = first + ext.size();
		ct.tolower(first, last);

		for (int i = 0, size = static_cast<int>(targets.Size()); i < size; ++i)
		{
			if (ext == targets[i].target)
				return i;
		}
		return -1;
	}



	/**
	 * @brief Convert integer to wide string.
	*/
	inline WideString GenerateId(std::atomic<long long>& id)
	{
		return std::to_wstring(id.fetch_add(1));
	}
}
