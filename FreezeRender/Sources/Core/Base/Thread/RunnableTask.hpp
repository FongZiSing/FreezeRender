#pragma once

#include <type_traits>
#include <concrt.h>



/**
 * @brief A runnable lightweiget async task base on window concurrency runtime library.
 * @see https://docs.microsoft.com/en-us/cpp/parallel/concrt/reference/currentscheduler-class?view=msvc-170#scheduletask
 * @see https://docs.microsoft.com/en-us/cpp/parallel/concrt/walkthrough-adapting-existing-code-to-use-lightweight-tasks?view=msvc-170
 */
class TinyRunnableTask
{
public:
	// the maximum number of milliseconds before the wait times out.
	enum : unsigned int { Timeout = (unsigned int)-1 /* 4294967295u */ };

private:
	// The task completion event.
	Concurrency::event signal;

	/**
	 * @brief The internal version of `DoWork()`.
	 */
	static void __cdecl DoWorkInternal(void* instance)
	{
		TinyRunnableTask* task = static_cast<TinyRunnableTask*>(instance);
		task->DoWork();
		task->signal.set();
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
		signal.reset();
		Concurrency::CurrentScheduler::ScheduleTask(DoWorkInternal, this);
	}

	/**
	 * @brief Waits for the task to become finished.
	 * @return        If the wait was satisfied, the value `0` is returned;
	 *                otherwise, the value `4294967295u` to indicate that the wait timed out.
	 */
	inline unsigned long long Wait(unsigned int timeout = Timeout)
	{
		return signal.wait(timeout);
	}
};