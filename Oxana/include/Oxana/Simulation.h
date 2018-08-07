#pragma once

#include "Window.h"
#include "WatchVariable.h"
#include "EditableVariable.h"
#include "Utilities.h"

namespace Oxana 
{		
	enum class StepType
	{
		Once,
		Multiple,
		Forward,
		Backward,
	};

	using ResetFunction = std::function<void()>;
	using StepFunction = std::function<bool()>;
	using DriverFunction = std::function<int(char, char*[])>;

	class Simulation 
	{
		friend class GUI;

		//---------------------------------------------------------------------------/
		// Fields
		//---------------------------------------------------------------------------/
		public:
		// The name of the simulation
		std::string name;
		// If there's a category for this simulation
		std::string menu;
		// Invoked when the simulation is reset
		ResetFunction onReset;
		// Invoked for every step of the simulation
		StepFunction onStep;
		// Shows the result of the simulation
		LogFunction onResult;
		// How many steps are stored at a time
		unsigned stepBufferSize;
		// The current step for this simulation
		int stepMultiplier;

		private:		
		// The number of steps in the current simulation
		unsigned steps;
		// The current step in the simulation
		int currentStep;
		// Whether the current simulation is done
		bool isDone;
		// Callback function when this simulation has finished
		std::function<void()> onDone;
		// Whether this simulation should run until its finished automatically
		bool isAutomatic;
		std::stack<StepFunction> substeps;
		// Windows
		std::vector<Window*> windows;
		std::vector<LogWindow> loggingWindows;
		std::vector<ColorMapWindow> colorMapWindows;
		std::vector<std::shared_ptr<NodeDisplayWindow>> nodeDisplayWindows;
		// Watched Variables
		std::vector<std::shared_ptr<WatchVariableBase>> watchVariables;
		std::vector<std::shared_ptr<PlotVariableBase>> plotVariables;
		// Notifications
		std::vector<NotificationWindow> persistentNotifications;
		std::string resultLog;
		std::shared_ptr<void> data;
		// Editable Variables
		std::vector<EditableFloat> editableFloatVariables;
		std::vector<EditableInt> editableIntVariables;
		std::vector<EditableBool> editableBoolVariables;

		//---------------------------------------------------------------------------/
		// CTOR
		//---------------------------------------------------------------------------/
		public:
		Simulation();
		Simulation(const std::string& name, StepFunction onStep, LogFunction onValidate, int stepMultiplier);		

		//---------------------------------------------------------------------------/
		// Methods
		//---------------------------------------------------------------------------/
		public:
		// Adds a subset function
		void AddSubstep(const StepFunction& stepFn) { this->substeps.push(stepFn); }
		// Adds a custom logging window
		void AddLog(std::string title, LogFunction printFn);
		// Adds a custom map window
		void AddMap(std::string title, ColorMap map, ColorMap::Function gridFn);
		// Notifies the user when a given condition is true, printing a message
		void AddNotification(const std::string& title, NotificationWindow::ConditionalFunction onCheckCondition, NotificationWindow::MessageFunction onNotify, bool persistent = false);
		// Notifies the user when a given condition is true, printing a message
		void AddNotification(const std::string& title, bool& condition, NotificationWindow::MessageFunction onNotify, bool persistent = false);
		// Tracks a value (by having the user provide a custom print function of it)
		void Watch(std::string name, LogFunction printValueFn);
		// Edits a value with the GUI at runtime
		void Edit(std::string name, float* value) { editableFloatVariables.push_back(EditableFloat(name, value, 0, 0, false)); }
		void Edit(std::string name, float* value, float min, float max) { editableFloatVariables.push_back(EditableFloat(name, value, min, max, true)); }
		void Edit(std::string name, int* value) { editableIntVariables.push_back(EditableInt(name, value, 0, 0, false)); }
		void Edit(std::string name, int* value, int min, int max) { editableIntVariables.push_back(EditableInt(name, value, min, max, true)); }
		void Edit(std::string name, bool* value) { editableBoolVariables.push_back(EditableBool(name, value)); }

		// Tracks a variable (by having the user provide a reference to it)
		template <typename ValueType>
		void Watch(std::string name, ValueType* variable)
		{
			WatchVariable<ValueType> watchedVariable(name, variable);
			watchVariables.push_back(std::make_shared<WatchVariable<ValueType>>(watchedVariable));
		}
		// Tracks and plots a float-castable variable over stepss
		template <typename NumericType>
		void PlotLine(std::string name, NumericType* variable)
		{
			PlotVariable<NumericType> plot(name, variable);
			plotVariables.push_back(std::make_shared<PlotVariable<NumericType>>(plot));
		}

		// Observes a given node on a graph
		template <typename NodeType>
		void ObserveNode(std::string title, GenericNodeDisplay<NodeType> nodeDisplay)
		{
			auto window = std::make_shared<NodeDisplayWindow>();
			window->title = title;
			window->display.reset(new GenericNodeDisplay<NodeType>(nodeDisplay));
			nodeDisplayWindows.push_back(window);
			windows.push_back(window.get());
		}

		template <typename T>
		T* SetData()
		{
			data = std::make_shared<T>();
			return static_cast<T*>(data.get());
		}

		private:
		int Min() { return (steps > stepBufferSize) ? steps - stepBufferSize + 1 : 0; }
		int Max() { return steps; }
		bool HasEditableVariables() const { return !editableBoolVariables.empty() || !editableFloatVariables.empty() || !editableIntVariables.empty(); }

		void Initialize();
		void Reset();
		void Step(const StepType& type);
		void OnStep();
		void Done();
		void Update();
		void SynchronizeBuffers();
		void RecordVariables();

	};

}