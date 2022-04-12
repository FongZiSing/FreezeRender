#pragma once

#include <type_traits>



template <class Derived>
class Singleton
{
protected:
	struct Token {};

	Singleton() noexcept = default;

public:
	static Derived& Get() noexcept(std::is_nothrow_constructible<Derived, Token>::value)
	{
		static Derived Instance{ Token() };
		return Instance;
	}

	Singleton(const Singleton&) = delete;

	Singleton& operator = (const Singleton&) = delete;

	~Singleton() = default;
};
