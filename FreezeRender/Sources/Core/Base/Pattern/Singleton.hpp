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


#include <memory>
template <class Derived>
class UniqueResource
{
private:
	std::unique_ptr<Derived> resource;
	//Derived* resource = nullptr;
	std::atomic<bool> flag = false;

public:
	UniqueResource(const UniqueResource&) = delete;

	UniqueResource& operator = (const UniqueResource&) = delete;

	UniqueResource() noexcept = default;

	~UniqueResource()
	{
		//if (resource)
		//{
		//	delete resource;
		//}
		//resource = nullptr;
		//flag.store(false);
	}

	template<class DerivedClass>
	constexpr void Initialize(DerivedClass* derivedClass) noexcept
	{
		resource.reset(derivedClass);
		//bool bNotInit = false;
		//if (flag.compare_exchange_strong(bNotInit, true))
		//{
		//	if (!resource)
		//	{
		//		resource = derivedClass;
		//	}
		//}
	}

	constexpr Derived* operator -> () const noexcept { return resource.get(); }
	
	constexpr Derived* operator * () const noexcept { return resource.get(); }
};