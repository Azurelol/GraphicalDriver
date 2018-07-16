#include "Examples.h"

namespace Oxana
{
	int SimpleExample()
	{
		struct Example {
			int apples;
			int pears;
			StringBuilder log;
		};

		// Used to configure the GUI
		Oxana::GUI::Settings settings;

		// Construct the gui
		Oxana::GUI gui(settings);

		// Your class being watched
		Example example;

		// Customize Oxana here  
		gui.settings.title = "Example Driver";
		gui.settings.width = 1440;
		gui.settings.height = 900;

		auto logFunction = [&]() -> std::string
		{
			return example.log.ToString();
		};

		// Simple simulation: Will keep running while there's more apples than pears
		auto stepFunction = [&]() -> bool
		{
			example.apples--;
			example.pears++;
			example.log.Clear();
			example.log.AppendLine("Apples = ", example.apples, ", Pears = ", example.pears);
			return (example.apples > example.pears);
		};

		auto resultFunction = [&]() -> std::string
		{
			return "Apples now equal to pears!";
		};

		auto resetFunction = [&]() {
			example.apples = 100;
			example.pears = 0;
			example.log.Clear();
		};

		// Construct the simulation
		Oxana::Simulation exampleSimulation;
		exampleSimulation.name = "Apples vs Pears";
		exampleSimulation.onStep = stepFunction;
		exampleSimulation.onReset = resetFunction;
		exampleSimulation.onResult = resultFunction;
		exampleSimulation.Watch("Apples", &example.apples);
		exampleSimulation.Watch("Pears", &example.pears);
		exampleSimulation.PlotLine("Apples", &example.apples);
		exampleSimulation.AddLog("Update", logFunction);

		gui.Add(exampleSimulation);

		// Run it!
		gui.Run();

		return 0;
	}

}