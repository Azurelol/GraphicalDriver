#include "Simulation.h"

namespace Oxana
{
	Simulation::Simulation() : name("Simulation"),
		steps(0),
		currentStep(steps),
		stepBufferSize(100),
		stepMultiplier(5),
		isDone(false)		
	{
		//AddNotification("Validation", [&]()-> bool { return isDone; }, [&]() -> std::string { return Validate(); });
	}

	Simulation::Simulation(const std::string & name, StepFunction onStep, LogFunction onValidate, int stepMultiplier)
		: name(name), onStep(onStep), onResult(onValidate), stepMultiplier(stepMultiplier)
	{
		//AddNotification("Validation", [&]()-> bool { return isDone; }, [&]() -> std::string { return Validate(); });
	}
	
	void Simulation::Initialize()
	{
		this->SynchronizeBuffers();
		this->Reset();
		//this->RecordVariables();
		//this->Update();
		//AddNotification("Validation", isDone, onValidate);
	}

	void Simulation::Reset()
	{
		if (onReset != nullptr) {
			onReset();
		}

		//this->persistentNotifications.clear();
		
		for (auto& window : this->windows) {
			window->Reset();
		}

		for (auto& var : this->plotVariables) {
			var->Clear();
		}

		for (auto& var : this->watchVariables) {
			var->Clear();
		}

		this->isAutomatic = false;
		this->steps = 0;
		this->currentStep = this->steps;
		this->isDone = false;

		this->RecordVariables();
		this->Update();
	}

	void Simulation::Step(const StepType & type)
	{
		switch (type)
		{
			case StepType::Once:
				OnStep();
				break;
			case StepType::Multiple:
				for (int i = 0; i < stepMultiplier; ++i)
					OnStep();
				break;
			case StepType::Forward:
				if (currentStep < Max())
					currentStep++;
				break;
			case StepType::Backward:
				if (currentStep > Min())
					currentStep--;
				break;
		}
	}

	void Simulation::OnStep()
	{
		if (isDone)
			return;

		if (!onStep)
		{
			return;
		}

		isDone = !onStep();
		if (isDone)
			this->Done();

		this->steps++;
		this->currentStep = steps;

		// Update all windows
		this->Update();

		// Record changed variables
		RecordVariables();

		//for (auto& log : loggingWindows)
		//	log.Update();
		//
		//for (auto& colorMap : colorMapWindows)
		//	colorMap.Update();
		//
		//for (auto& notification : persistentNotifications)
		//{
		//	notification.Update();
		//	if (notification.enabled)
		//		this->isAutomatic = false;
		//}
		//
		//for (auto& window : windows)
		//	window->Update();

	}

	//std::string Simulation::Validate()
	//{
	//	if (onResult) {
	//		return onValidate();
	//	}
	//	return std::string();
	//}

	void Simulation::Done()
	{
		this->resultLog = onResult != nullptr ? onResult() : "Simulation finished!";
		if (onDone) {
			onDone();
		}
	}

	void Simulation::Update()
	{
		for (auto& window : windows)
		{
			window->Update();
		}

		for (auto& notification : persistentNotifications)
		{
			if (notification.enabled)
				this->isAutomatic = false;
		}
	}

	void Simulation::SynchronizeBuffers()
	{
		for (auto& plot : plotVariables)
		{
			plot->bufferSize = stepBufferSize;
			plot->history.reserve(plot->bufferSize);
		}

		for (auto& log : loggingWindows)
		{
			log.bufferSize = stepBufferSize;
		}

		for (auto& window : windows)
		{
			window->bufferSize = stepBufferSize;
		}
	}

	void Simulation::RecordVariables()
	{
		// Record all variables as needed
		for (auto& var : plotVariables)
		{
			var->Record();
		}
		for (auto& var : watchVariables)
		{
			var->Record();
		}
	}

	void Simulation::AddLog(std::string title, LogFunction printFn)
	{
		LogWindow window;
		window.title = title;
		window.function = printFn;
		loggingWindows.push_back(window);
		windows.push_back(&loggingWindows.back());
	}

	void Simulation::AddMap(std::string title, ColorMap map, ColorMap::Function gridFn)
	{
		ColorMapWindow window;
		window.title = title;
		window.function = gridFn;
		window.map = map;
		colorMapWindows.push_back(window);
		windows.push_back(&colorMapWindows.back());
	}

	void Simulation::AddNotification(const std::string & title, NotificationWindow::ConditionalFunction onCheckCondition, NotificationWindow::MessageFunction onNotify, bool persistent)
	{
		NotificationWindow window;
		window.enabled = false;
		window.title = title;
		window.onCheckCondition = onCheckCondition;
		window.onNotify = onNotify;
		window.persistent = persistent;
		persistentNotifications.push_back(window);
		windows.push_back(&persistentNotifications.back());
	}

	void Simulation::AddNotification(const std::string & title, bool & condition, NotificationWindow::MessageFunction onNotify, bool persistent)
	{
		auto conditionalFn = [&]() -> bool { return condition; };
		AddNotification(title, conditionalFn, onNotify, persistent);
	}

	void Simulation::Watch(std::string name, LogFunction printValueFn)
	{
		PrintVariableValue printVariable(name, printValueFn);
		watchVariables.push_back(std::make_shared<PrintVariableValue>(printVariable));
	}

}