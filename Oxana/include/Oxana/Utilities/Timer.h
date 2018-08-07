#pragma once

#include <chrono>
#include <climits>

namespace Oxana
{
	//--------------------------------------------------------------------------/
	// Declarations
	//--------------------------------------------------------------------------/
	// The clock being used for real physical time
	using Clock = std::chrono::high_resolution_clock;
	// A reference to a specific point in time, like one's birthday, etc
	//  high_resolution_clock is the clock with the shortest tick period
	using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
	// Durations measure time spans, like one minute, two hours, ten ms
	using FloatSeconds = std::chrono::duration<float>;

	class Timer
	{
		TimePoint startTime;

		protected:
		Timer() : startTime(Clock::now())
		{
		}

		// Returns the time elapsed since this timer was created
		float Elapsed()
		{
			auto currentTime = Clock::now();
			FloatSeconds duration = currentTime - startTime;
			return duration.count();
		}
	};

	// When the timer leaves the scope, gets the end time. It then
	// converts the duration to a float where 1.0f == 1 second.
	// It then stores the result in a pointer given at initialization.
	class ScopeTimer : public Timer
	{
		private:

		float* returnValue;
		bool isReturning;

		public:
		ScopeTimer(float* output) : returnValue(output), isReturning(true) {}
		ScopeTimer() : isReturning(false) {}
		~ScopeTimer()
		{
			// If a float reference was passed in, this timer will set it now
			if (isReturning)
				*returnValue = Elapsed();
		}
	};
}