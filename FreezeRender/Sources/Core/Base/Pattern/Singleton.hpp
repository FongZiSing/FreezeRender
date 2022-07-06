#pragma once

#include <Common.hpp>
#include <type_traits>
#include <Thread/SpinLock.hpp>



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
	Derived* resource;
	SpinLock scope;

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
	}

	template<class DerivedClass, typename ...Args>
	constexpr void Initialize(Args&&... agrs) noexcept
	{
		scope.Lock();
		if (!resource)
		{
			resource = new DerivedClass(std::forward<Args>(agrs)...);
		}
		scope.Unlock();
	}

	inline void Swap(UniqueResource& rhs) noexcept
	{
		scope.Lock();
		{
			Derived* temp = resource;
			resource = rhs.resource;
			rhs.resource = temp;
		}
		scope.Unlock();
	}

	constexpr Derived* operator -> () const noexcept { return resource; }
	
	constexpr Derived* operator * () const noexcept { return resource; }
};