#include "UnitTest.h"

static int bufferSize = 9999;

namespace Oxana 
{
	Test::Test() : enabled(false)
	{	
		Initialize();
	}
	
	Test::Test(const std::string & name) : name(name), enabled(false)
	{
		Initialize();
	}

	void Test::Initialize()
	{
		window.function = [&]() { return this->output; };
		window.enabled = this->enabled;
		window.bufferSize = bufferSize;
	}

	void Test::Run()
	{
		window.title = name;
		UnitTest unitTest = this->function();
		this->output = unitTest.ToString();
	}

}