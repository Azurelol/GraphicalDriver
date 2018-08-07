#include "UnitTest.h"

static int bufferSize = 9999;

namespace Oxana
{
	//Test::Test(const std::string & name, UnitTestFunction testFunction) : name(name), function(testFunction), enabled(false), type(TestFunctionType::CreateTest)
	//{
	//	Initialize();
	//}

	Test::Test(const std::string & name, UnitTestFunction2 testFunction) : name(name), function2(testFunction), enabled(false), type(TestFunctionType::PassTest)
	{
		Initialize();
	}

	void Test::Initialize()
	{
		//window.function = [&]() { return this->output; };
		//window.enabled = this->enabled;
		//window.bufferSize = bufferSize;
	}

	void Test::Run()
	{
		//window.title = name;

		switch (this->type)
		{
			//case TestFunctionType::CreateTest:
			//{
			//	UnitTest unitTest = this->function();
			//	this->output = unitTest.ToString();
			//	this->images = unitTest.images;
			//}
			//break;

			case TestFunctionType::PassTest:
			{
				UnitTest unitTest;
				this->function2(unitTest);
				unitTest.OnRun();
				this->output = unitTest.result;
			}
			break;
		}

		


	}

}