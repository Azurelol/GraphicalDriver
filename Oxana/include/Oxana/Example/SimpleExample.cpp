#include "Examples.h"

namespace Oxana
{	
	namespace Examples
	{
		void ApplesPears(GUI& gui)
		{
			struct ApplesPears
			{
				int apples;
				int pears;
				StringBuilder log;
			};

			// Your class being watched
			static ApplesPears example;

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

			auto resetFunction = [&]()
			{
				example.apples = 100;
				example.pears = 0;
				example.log.Clear();
			};

			// Construct the simulation
			static Oxana::Simulation simulation;
			simulation.name = "Apples vs Pears";			
			simulation.onStep = stepFunction;
			simulation.onReset = resetFunction;
			simulation.onResult = resultFunction;
			simulation.Watch("Apples", &example.apples);
			simulation.Watch("Pears", &example.pears);
			simulation.PlotLine("Apples", &example.apples);
			simulation.AddLog("Update", logFunction);

			gui.Add(simulation);
		}
	}


}