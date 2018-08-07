#include "UnitTest.h"

static int bufferSize = 9999;

namespace Oxana
{
	//Test::Test(const std::string & name, UnitTestFunction testFunction) : name(name), function(testFunction), enabled(false), type(TestFunctionType::CreateTest)
	//{
	//	Initialize();
	//}

	TestRunner::TestRunner(const std::string & name, UnitTestFunction testFunction) : name(name), function(testFunction), enabled(false), type(TestFunctionType::PassTest)
	{
		Initialize();
	}

	void TestRunner::Initialize()
	{
	}

	void TestRunner::Run()
	{

		switch (this->type)
		{
			case TestFunctionType::PassTest:
			{
				UnitTest unitTest;
				this->function(unitTest);
				this->output = unitTest.ToResult();
			}
			break;
		}
	}

	void UnitTest::AppendImage(const std::string & name, const std::string & path)
	{
		this->SubmitContentIfNeeded();

		Texture image = Texture(name, path);
		ImageContent content(image);
		content.text = name;

		this->result.content.push_back(std::make_shared<ImageContent>(content));
	}

	void UnitTest::AppendImage(const std::string & name, const Image & image)
	{
		this->SubmitContentIfNeeded();

		Texture texture(image.ToTexture());
		ImageContent content(texture);
		content.text = name;

		this->result.content.push_back(std::make_shared<ImageContent>(content));
	}

	void UnitTest::Time(ProcedureFunction procedure)
	{
		this->BeginTimer();
		procedure();
		this->EndTimer();
	}

	void UnitTest::BeginTimer()
	{
		if (timing)
			throw std::exception("The timer is already running!");

		timing = true;
		timer.Reset();

		Content content;
		content.type = Content::Type::TimerStarted;
		this->result.content.push_back(std::make_shared<Content>(content));
	}

	void UnitTest::EndTimer()
	{
		if (!timing)
			throw std::exception("The timer must be started before it can be ended");

		float elapsed = timer.Elapsed();
		timing = false;

		Content content;
		//content.text = "Completed in " + std::to_string(elapsed) + " seconds";
		content.text = "(" + std::to_string(elapsed) + " seconds)";
		content.type = Content::Type::TimerEnded;
		this->result.content.push_back(std::make_shared<Content>(content));
	}
	
	void UnitTest::SubmitContent()
	{
		std::string text = this->ToString();
		this->Clear();

		Content content;
		content.text = text;
		this->result.content.push_back(std::make_shared<Content>(content));
	}

	UnitTest::Result UnitTest::ToResult()
	{
		if (!evaluated)
			this->OnRun();

		return result;
	}

	void UnitTest::OnRun()
	{
		this->SubmitContentIfNeeded();
		if (this->timing)
			this->EndTimer();
		this->evaluated = true;
	}

	void UnitTest::SubmitContentIfNeeded()
	{
		if (!this->IsEmpty())
			this->SubmitContent();
	}



}