#pragma once

#include<atomic>



struct SpinLock
{
private:
	std::atomic<bool> flag{ false };

	inline void YieldImpl()
	{
		_mm_pause();
	}

public:
	SpinLock() noexcept = default;

	SpinLock(const SpinLock&) = delete;

	SpinLock& operator = (const SpinLock&) = delete;

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
				YieldImpl();
			}
		}
	}

	bool Trylock()
	{
		return flag.exchange(true, std::memory_order_acquire);
	}

	void Unlock()
	{
		flag.store(false, std::memory_order_release);
	}
};