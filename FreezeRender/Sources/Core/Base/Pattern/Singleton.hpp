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
	Derived* resource;

#pragma region SpinLock
__pragma(push_macro("Yield"))
#undef Yield

	std::atomic<bool> flag{ false };

	void Yield()
	{
		_mm_pause();
	}

	void Lock()
	{
		while (true)
		{
			if (!flag.exchange(true, std::memory_order_acquire))
			{
				break;
			}

			while (flag.load(std::memory_order_relaxed))
			{
				Yield();
			}
		}
	}

	void Unlock()
	{
		flag.store(false, std::memory_order_release);
	}

__pragma(pop_macro("Yield"))
#pragma endregion SpinLock

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
		flag.store(false, std::memory_order_release);
	}

	template<class DerivedClass, typename ...Args>
	constexpr void Initialize(Args&&... agrs) noexcept
	{
		Lock();
		if (!resource)
		{
			resource = new DerivedClass(std::forward<Args>(agrs)...);
		}
		Unlock();
	}

	inline void Swap(UniqueResource& rhs) noexcept
	{
		Lock();
		{
			Derived* temp = resource;
			resource = rhs.resource;
			rhs.resource = temp;
		}
		Unlock();
	}

	constexpr Derived* operator -> () const noexcept { return resource; }
	
	constexpr Derived* operator * () const noexcept { return resource; }
};