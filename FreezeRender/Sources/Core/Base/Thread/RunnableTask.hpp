//
// Thread/RunableTask.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Common parallel operation.
//
#pragma once

#include <Platform/Async.hpp>



namespace Pluto
{
	/**
	 * @brief Common thread utility.
	 */
	namespace Thread
	{
		class TinyRunnableTask
		{
		public:
			// the maximum number of milliseconds before the wait times out.
			enum : unsigned int { Timeout = (unsigned int)-1 /* 4294967295u */ };

		private:
			// The task completion event.
			Platform::TinyTask task;

			/**
			 * @brief The internal version of `DoWork()`.
			 */
			static void __cdecl DoWorkInternal(void* instance)
			{
				TinyRunnableTask* my = static_cast<TinyRunnableTask*>(instance);
				my->DoWork();
				my->task.Finish();
			}

		protected:
			/**
			 * @brief Override this function to do you job.
			 */
			virtual void DoWork() = 0;

		public:
	
			/**
			 * @brief Start the async task, please make sure the task is not running now.
			 */
			inline void Start()
			{
				task.Start(DoWorkInternal, this);
			}

			/**
			 * @brief Waits for the task to become finished.
			 * @return        If the wait was satisfied, the value `0` is returned;
			 *                otherwise, the value `4294967295u` to indicate that the wait timed out.
			 */
			inline unsigned long long Wait(unsigned int timeout = Timeout)
			{
				return task.Wait(timeout);
			}
		};
	}
}