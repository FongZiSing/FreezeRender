//
// Thread/SpinLock.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Mutex.
//
#pragma once

#include<atomic>



namespace Pluto
{
	/**
	 * @brief Common thread utility.
	 */
	namespace Thread
	{
		/**
		 * @brief A mutex that doesn't put the thread into a WAIT state but instead repeatedly tries to aquire the lock.
		 * @note Should be used only for very short locks
		 */
		struct AtomicSpinLock
		{
		private:
			std::atomic<bool> flag{ false };

		public:
			
			AtomicSpinLock() noexcept = default;

			//--------------------------------
			//~ Begin non-copyable.

			AtomicSpinLock(const AtomicSpinLock&) = delete;
			AtomicSpinLock& operator = (const AtomicSpinLock&) = delete;

			//~ End non-copyable.
			//--------------------------------

			
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
						//std::this_thread::yield();
						_mm_pause();
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
	}
}