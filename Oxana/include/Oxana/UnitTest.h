#pragma once

#include "Window.h"
#include "Utilities.h"
#include "WatchVariable.h"

namespace Oxana
{
	struct UnitTest : StringBuilder
	{
		//struct Check {
		//	std::string output;
		//	bool pass;
		//};

		bool Assert(float result, float expected, float epsilon) 
		{
			bool valid = WithinRange(result, expected, epsilon);
			AppendLine("Result = ", result, ", Expected = ", expected, ", ", valid ? "OK" : "FAIL");
			return valid;
		}

		template <typename T>	bool Assert(T result, T expected)
		{
			bool valid = WithinRange(result, , expected);
			AppendLine("Result = ", result, ", Expected = ", expected, ", ", valid ? "OK" : "FAIL");
			return valid;
		}

		static bool WithinRange(float value, float expected, float epsilon) 
		{
			return std::abs(value - expected) < epsilon;
		}

	};

	using TestFunction = std::function<UnitTest(void)>;

	class Test
	{
		friend class GUI;

		public:
		std::string name;
		TestFunction function;
	
		private:
		std::string output;
		bool enabled;
		LogWindow window;
		float timeElapsed;	
	
		public:
		Test(); 
		Test(const std::string& name);	
	
		private:
		void Initialize();
		void Run();
	
	};

	

}