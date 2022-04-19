#pragma once

#include <Common.hpp>
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



template <class Derived>
class UniqueResource
{
private:
	static Derived resource;

public:
	UniqueResource() noexcept = default;

	~UniqueResource() = default;

	UniqueResource(const UniqueResource&) = delete;

	UniqueResource& operator = (const UniqueResource&) = delete;

	constexpr Derived* operator -> () const noexcept { return &resource; }
};

template<class Derived> inline_variable Derived UniqueResource<Derived>::resource;