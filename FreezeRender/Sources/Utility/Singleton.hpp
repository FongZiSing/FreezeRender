#pragma once

#include <type_traits>



template <typename Derived>
class Singleton
{
protected:
	struct Token {};

	Singleton() noexcept = default;

public:
	static Derived* Instance() noexcept(std::is_nothrow_constructible<Derived, Token>::value)
	{
		static Derived D{ Token() };
		return &D;
	}

	Singleton(const Singleton&) = delete;

	Singleton& operator = (const Singleton&) = delete;

	~Singleton() = default;
};
