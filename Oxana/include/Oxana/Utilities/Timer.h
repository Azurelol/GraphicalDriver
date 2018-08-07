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

		public:
		using Callback = std::function<void(float)>;

		Timer() : startTime(Clock::now())
		{
		}

		// Resets the timer to the current time
		void Reset()
		{
			startTime = Clock::now();
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
		enum class ReturnType
		{
			Callback,
			UpdateValue
		};
		float* returnValue;
		ReturnType type;
		Callback callback;		

		public:
		ScopeTimer(float* output) : returnValue(output), type(ReturnType::UpdateValue) {}
		ScopeTimer(Callback onFinished) : callback(callback), type(ReturnType::Callback) {}
		~ScopeTimer()
		{
			float elapsed = Elapsed();
			switch (this->type)
			{
				case ReturnType::Callback:
					callback(elapsed);

				case ReturnType::UpdateValue:
					*returnValue = elapsed;
			}
		}
	};
}