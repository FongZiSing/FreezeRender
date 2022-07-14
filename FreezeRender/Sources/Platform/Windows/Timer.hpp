#pragma once

#include <Common.hpp>
#include <cmath>

// Windows Header.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>



namespace Platform
{
	/**
	 * @brief time counter.
	 */
	class WindowTimer
	{
	public:

		WindowTimer()
		{
			m_secondsPerCount = 1000.0 / static_cast<double>(GetPerformanceFrequency());
		}

		warn_nodiscard constexpr double TotalTime() const
		{
			return m_bStopped
				? ((m_stopTime - m_pausedTime) - m_baseTime) * m_secondsPerCount
				: ((m_currTime - m_pausedTime) - m_baseTime) * m_secondsPerCount;
		}

		warn_nodiscard constexpr float GetDeltaTime() const
		{
			return static_cast<float>(m_deltaTime);
		}

		warn_nodiscard constexpr float GetElapsedTime() const
		{
			return static_cast<float>(m_timeElapsed);
		}

		warn_nodiscard constexpr unsigned int GetFrameNumber() const
		{
			return m_framePerElapsed;
		}

		force_inline void Reset()
		{
			__int64 tempCurTime = GetPerformanceCounter();
			m_baseTime = tempCurTime;
			m_prevTime = tempCurTime;
			m_stopTime = 0;
			m_bStopped = false;
		}

		force_inline void Start()
		{
			if (m_bStopped)
			{
				__int64 tempCurTime = GetPerformanceCounter();
				m_pausedTime += (tempCurTime - m_stopTime);
				m_prevTime = tempCurTime;
				m_stopTime = 0;
				m_bStopped = false;
			}
		}

		force_inline void Stop()
		{
			if (!m_bStopped)
			{
				__int64 TempCurTime = GetPerformanceCounter();
				m_stopTime = TempCurTime;
				m_bStopped = true;
			}
		}

		inline void BeginFrame()
		{
			if (m_bStopped)
			{
				m_deltaTime = 0.0;
				return;
			}

			__int64 tempCurTime = GetPerformanceCounter();
			m_currTime = tempCurTime;
			m_deltaTime = (m_currTime - m_prevTime) * m_secondsPerCount;
			m_deltaTime = std::max(0.0, m_deltaTime);
			m_prevTime = m_currTime;

			m_timeElapsed += m_deltaTime;
			m_currFrame++;
		}

		inline void EndFrame()
		{
			constexpr double elapsed = 1000.0;
			if (m_timeElapsed > elapsed)
			{
				m_timeElapsed = 0;
				m_framePerElapsed = m_currFrame - m_lastFrame;
				m_lastFrame = m_currFrame;
			}
		}


	private:
		force_inline __int64 GetPerformanceFrequency() const
		{
			__int64 frequency;
			QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
			return frequency;
		}

		force_inline __int64 GetPerformanceCounter() const
		{
			__int64 counter;
			QueryPerformanceCounter((LARGE_INTEGER*)&counter);
			return counter;
		}



		bool m_bStopped = false;

		double m_secondsPerCount = 0;
		double m_deltaTime = -1.0;

		__int64	m_baseTime = 0;
		__int64	m_pausedTime = 0;
		__int64	m_stopTime = 0;
		__int64	m_prevTime = 0;
		__int64	m_currTime = 0;


		double m_timeElapsed = 0.0;
		unsigned int m_lastFrame = 0;
		unsigned int m_currFrame = 0;
		unsigned int m_framePerElapsed = 0;
	};
}