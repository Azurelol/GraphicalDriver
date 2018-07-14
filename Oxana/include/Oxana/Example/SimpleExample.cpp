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
    example.apples = 100;
    example.pears = 0;

    // Customize Oxana here  
    gui.settings.title = "Example Driver";
    gui.settings.width = 1440;
    gui.settings.height = 900;
    gui.settings.stepBufferSize = 100;

    gui.Watch("Apples", &example.apples);
    gui.Watch("Pears", &example.pears);
    gui.PlotLine("Apples", &example.apples);

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

	auto validateFunction = [&]() -> std::string
	{
		return "Example finished!";
	};

    gui.AddLog("Update", logFunction);
    gui.SetStep(stepFunction);
	gui.settings.onValidate = validateFunction;

    // Run it!
    gui.Run();

    return 0;	
  }

}