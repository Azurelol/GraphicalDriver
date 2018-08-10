#pragma once

#include "Window.h"
#include "Utilities.h"
#include "WatchVariable.h"

namespace Oxana
{
	class UnitTest : public StringBuilder
	{
		friend class TestRunner;

		public:
		//---------------------------------------------------------------------------/
		// Declarations
		//---------------------------------------------------------------------------/
		struct Content
		{
			public:
			enum class Type
			{
				Text,
				Assertion,
				Image,
				TimerStarted,
				TimerEnded
			};

			Type type;
			std::string text;
			Content() : type(Type::Text) {}
			Content(Type type) : type(type) {}
			virtual ~Content() {}
		};

		struct Assertion : Content
		{
			bool pass;
			Assertion() : Content(Type::Assertion) {}
		};

		struct ImageContent : Content
		{
			Texture image;
			ImageContent(const Texture& image) : image(image), Content(Type::Image) {}
		};

		struct Result
		{
			int passedAssertions;
			int totalAssertions;
			std::vector<std::shared_ptr<Content>> content;

			Result() : passedAssertions(0), totalAssertions(0) {}
		};


		//---------------------------------------------------------------------------/
		// Fields
		//---------------------------------------------------------------------------/
		private:
		Result result;
		Timer timer;
		bool timing;
		bool evaluated;

		//---------------------------------------------------------------------------/
		// Methods
		//---------------------------------------------------------------------------/
		public:
		using ProcedureFunction = std::function<void()>;

		UnitTest() : timing(false), evaluated(false)
		{
		}

		bool Assert(float result, float expected, float epsilon)
		{
			this->SubmitContentIfNeeded();

			bool valid = WithinRange(result, expected, epsilon);
			AppendLine("Result = ", result, ", Expected = ", expected, ", ", valid ? "OK" : "FAIL");

			Assertion content;
			content.text = this->ToString(); this->Clear();
			content.pass = valid;
			this->result.content.push_back(std::make_shared<Assertion>(content));

			if (valid)
				this->result.passedAssertions++;
			this->result.totalAssertions++;
			return valid;
		}

		template <typename T>	bool Assert(T result, T expected)
		{
			this->SubmitContentIfNeeded();

			bool valid = result == expected;
			AppendLine("Result = ", result, ", Expected = ", expected, ", ", valid ? "OK" : "FAIL");

			Assertion assertion;
			assertion.text = this->ToString(); this->Clear();
			assertion.pass = valid;
			this->result.content.push_back(std::make_shared<Assertion>(assertion));

			if (valid)
				this->result.passedAssertions++;
			this->result.totalAssertions++;
			return valid;
		}

		// Adds an image to the test from a gigven path
		void AppendImage(const std::string& name, const std::string& path);
		// Adds an image to the test
		void AppendImage(const std::string& name, Image& image);
		// Timers the given procedure
		void Time(ProcedureFunction procedure);
		// Begins the timer
		void BeginTimer();
		// Ends the current timer
		void EndTimer();

		private:
		void OnRun();
		void SubmitContent();
		void SubmitContentIfNeeded();
		// Returns the result of this test
		Result ToResult();

		//---------------------------------------------------------------------------/
		// Methods: Utlity
		//---------------------------------------------------------------------------/
		public:
		static bool WithinRange(float value, float expected, float epsilon)
		{
			return std::abs(value - expected) < epsilon;
		}


	};

	// A function that builds an unit test
	using UnitTestFunction = std::function<void(UnitTest&)>;

	class TestRunner
	{
		enum class TestFunctionType
		{
			CreateTest,
			PassTest
		};

		friend class GUI;

		public:
		std::string name;

		private:
		TestFunctionType type;
		UnitTestFunction function;
		UnitTest::Result output;
		std::vector<Texture> images;
		bool enabled;
		float timeElapsed;

		public:
		TestRunner(const std::string& name, UnitTestFunction testFunction);

		private:
		void Initialize();
		void Run();

	};



}