#pragma once

#include "Window.h"
#include "Utilities.h"
#include "WatchVariable.h"

namespace Oxana
{
	class UnitTest : StringBuilder
	{
		//---------------------------------------------------------------------------/
		// Declarations
		//---------------------------------------------------------------------------/
		struct Unit
		{
			std::string output;
		};

		struct Assertion : Unit
		{			
			bool pass;
		};

		//---------------------------------------------------------------------------/
		// Fields
		//---------------------------------------------------------------------------/
		int passedAssertions;
		int totalAssertions;

		//---------------------------------------------------------------------------/
		// Methods
		//---------------------------------------------------------------------------/
		UnitTest() : passedAssertions(0), totalAssertions(0)
		{
		}

		bool Assert(float result, float expected, float epsilon) 
		{
			bool valid = WithinRange(result, expected, epsilon);
			AppendLine("Result = ", result, ", Expected = ", expected, ", ", valid ? "OK" : "FAIL");
			if (valid)
				passedAssertions++;
			totalAssertions++;
			return valid;
		}

		template <typename T>	bool Assert(T result, T expected)
		{
			bool valid = WithinRange(result, , expected);
			AppendLine("Result = ", result, ", Expected = ", expected, ", ", valid ? "OK" : "FAIL");
			if (valid)
				passedAssertions++;
			totalAssertions++;
			return valid;
		}

		static bool WithinRange(float value, float expected, float epsilon) 
		{
			return std::abs(value - expected) < epsilon;
		}

	};

	using TestFunction = std::function<UnitTest(void)>;

	class TestSuite
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
		TestSuite(); 
		TestSuite(const std::string& name);	
	
		private:
		void Initialize();
		void Run();
	
	};

	

}