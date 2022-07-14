#pragma once

#include <concrt.h>



namespace Platform
{
	/**
	 * @brief A runnable lightweiget async task base on window concurrency runtime library.
	 * @see https://docs.microsoft.com/en-us/cpp/parallel/concrt/reference/currentscheduler-class?view=msvc-170#scheduletask
	 * @see https://docs.microsoft.com/en-us/cpp/parallel/concrt/walkthrough-adapting-existing-code-to-use-lightweight-tasks?view=msvc-170
	 */
	class TinyTask
	{
		Concurrency::event signal;

	public:
		inline void Start(Concurrency::TaskProc task, void* data)
		{
			signal.reset();
			Concurrency::CurrentScheduler::ScheduleTask(task, data);
		}

		inline void Finish()
		{
			signal.set();
		}

		inline size_t Wait(unsigned int timeout)
		{
			return signal.wait(timeout);
		}
	};
}