#pragma once

#include <iostream>
#include <sstream>
#include <chrono>
#include <climits>
#include <cstdint>
#include <type_traits>

#include "Utilities\Grid.h"
#include "Utilities\StringBuilder.h"
#include "Utilities\Parser.h"
#include "Utilities\Graph.h"

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

	class ScopeTimer
	{

		private:
		TimePoint StartTime;
		TimePoint EndTime;
		float* ReturnValue;
		bool IsReturning;

		public:

		ScopeTimer(float* output) : StartTime(Clock::now()), ReturnValue(output), IsReturning(true) {}
		ScopeTimer() : StartTime(Clock::now()), IsReturning(false) {}

		// When the timer leaves the scope, gets the end time. It then
		// converts the duration to a float where 1.0f == 1 second.
		// It then stores the result in a pointer given at initialization.
		~ScopeTimer()
		{
			// If a float reference was passed in, this timer will set it now
			if (IsReturning)
				*ReturnValue = Elapsed();
		}

		// Returns the time elapsed since this timer was created
		float Elapsed()
		{
			auto currentTime = Clock::now();
			FloatSeconds duration = currentTime - StartTime;
			return duration.count();
		}

	};





}