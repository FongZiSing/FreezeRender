#pragma once

#ifndef PLATFORM_COMMON_H
#define PLATFORM_COMMON_H



// FreezeEngine is available only with windows platform and visual studio compiler.
#if defined(WIN32) || defined(_WIN32) || defined(_WIN32_) || defined(WIN64) || defined(_WIN64) || defined(_WIN64_)
	#ifdef _MSC_VER
		#define PLATFORM_WINDOWS 1
	#else
		#error "[FreezeEngine] unsupported compiler!"
	#endif

#elif defined(ANDROID) || defined(_ANDROID_)
	#error "[FreezeEngine] unsupported android platform!"

#elif defined(__linux__)
	#error "[FreezeEngine] unsupported linux platform!"

#elif defined(__APPLE__) || defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_MAC)
	#error "[FreezeEngine] unsupported ios platform!"

#else
	#error "[FreezeEngine] unsupported unknown platform!"

#endif



// FreezeEngine is available only with C++20 or later.
#if defined(_MSVC_LANG) && !(_MSVC_LANG > 201703L) || !defined(__cplusplus)
	#error "[FreezeEngine] C++20 compiler required!"
#endif



// C++ RunTime Header.
#include <cassert>



// Re-define some keyword.
#ifndef force_inline
#define force_inline __forceinline
#endif

#ifndef force_noinline
#define force_noinline __declspec(noinline)
#endif

#ifndef scope_restrict
#define scope_restrict __restrict
#endif

#ifndef return_restrict
#define return_restrict __declspec(restrict)
#endif

#ifndef warn_nodiscard
#define warn_nodiscard [[nodiscard]]
#endif

#ifndef warn_deprecated
#define warn_deprecated(message) [[deprecated(message)]]
#endif

#ifndef force_softbreak
#define force_softbreak *(volatile int*)0 = 0
#endif

#ifndef interface_as
#define interface_as __declspec(novtable)
#endif

#ifndef allocator_as
#define allocator_as __declspec(allocator)
#endif

#ifndef without_globalvar
#define without_globalvar __declspec(noalias)
#endif

#ifndef inline_variable
#define inline_variable __declspec(selectany)
#endif

#ifndef branch_likely
#define branch_likely [[likely]]
#endif

#ifndef branch_unlikely
#define branch_unlikely [[unlikely]]
#endif



// All of the warnings that we are able to enable
#pragma warning(default: 4996) // 'function' was declared deprecated.



#endif // !PLATFORM_COMMON_H