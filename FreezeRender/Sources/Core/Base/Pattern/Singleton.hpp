#pragma once

#include <Common.hpp>
#include <type_traits>
#include <atomic>



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
	Derived* resource = nullptr;
	std::atomic<bool> flag = false;

public:
	UniqueResource(const UniqueResource&) = delete;

	UniqueResource& operator = (const UniqueResource&) = delete;

	UniqueResource() noexcept = default;

	~UniqueResource()
	{
		if (resource)
		{
			delete resource;
		}
		resource = nullptr;
		flag = false;
	}

	template<class DerivedClass>
	constexpr void Initialize(DerivedClass* derivedClass) noexcept
	{
		bool oldFlag = flag.load();
		if (flag.compare_exchange_strong(oldFlag, true))
		{
			if (!resource)
			{
				resource = derivedClass;
			}
		}
	}

	constexpr Derived* operator -> () const noexcept { return resource; }
	
	constexpr Derived* operator * () const noexcept { return resource; }
};