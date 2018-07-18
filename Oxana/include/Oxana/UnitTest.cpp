#include "UnitTest.h"

static int bufferSize = 9999;

namespace Oxana 
{
	TestSuite::TestSuite() : enabled(false)
	{	
		Initialize();
	}
	
	TestSuite::TestSuite(const std::string & name) : name(name), enabled(false)
	{
		Initialize();
	}

	void TestSuite::Initialize()
	{
		window.function = [&]() { return this->output; };
		window.enabled = this->enabled;
		window.bufferSize = bufferSize;
	}

	void TestSuite::Run()
	{
		window.title = name;
		UnitTest unitTest = this->function();
		this->output = unitTest.ToString();
	}

}