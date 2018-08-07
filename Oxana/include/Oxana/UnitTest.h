#pragma once

#include "Window.h"
#include "Utilities.h"
#include "WatchVariable.h"

namespace Oxana
{
	class UnitTest : public StringBuilder
	{
		public:
		//---------------------------------------------------------------------------/
		// Declarations
		//---------------------------------------------------------------------------/
		struct Content
		{
			enum class Type
			{
				Text,
				Assertion,
				Image
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
			Image image;
			ImageContent(const Image& image) : image(image), Content(Type::Image) {}
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
		Result result;		

		//---------------------------------------------------------------------------/
		// Methods
		//---------------------------------------------------------------------------/
		UnitTest()
		{
		}

		// Invoked after a series of append line calls in order to form a text content out of them
		void SubmitContent()
		{
			std::string text = this->ToString(); 
			this->Clear();

			Content content;
			content.text = text;
			this->result.content.push_back(std::make_shared<Content>(content));
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

		void AppendImage(const std::string& name, const std::string& path)
		{
			this->SubmitContentIfNeeded();

			Image image = Image(name, path);
			ImageContent content(image);
			content.text = name;

			this->result.content.push_back(std::make_shared<ImageContent>(content));
		}

		void OnRun()
		{
			this->SubmitContentIfNeeded();
		}

		void SubmitContentIfNeeded()
		{
			if (!this->IsEmpty())
				this->SubmitContent();
		}

		//---------------------------------------------------------------------------/
		// Methods: Utlity
		//---------------------------------------------------------------------------/
		static bool WithinRange(float value, float expected, float epsilon)
		{
			return std::abs(value - expected) < epsilon;
		}


	};

	// A function that returns a constructed unit test
	//using UnitTestFunction = std::function<UnitTest(void)>;
	// A function that builds an unit test
	using UnitTestFunction2 = std::function<void(UnitTest&)>;

	class Test
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
		//UnitTestFunction function;
		UnitTestFunction2 function2;
		UnitTest::Result output;
		std::vector<Image> images;
		bool enabled;
		//LogWindow window;
		float timeElapsed;	
	
		public:
		//Test(const std::string& name, UnitTestFunction testFunction);
		Test(const std::string& name, UnitTestFunction2 testFunction);
	
		private:
		void Initialize();
		void Run();
	
	};

	

}