#include "Examples.h"

namespace Oxana
{
	int RunExampleDriver()
	{
		// Used to configure the GUI
		GUI::Settings settings;
		settings.title = "Example Driver";
		settings.width = 1440;
		settings.height = 900;

		// Construct the gui
		GUI gui(settings);

		// Add examples
		using namespace Examples;
		ApplesPears(gui);
		SoftwareRendering(gui);

		// Run it!
		gui.Run();

		return 0;
	}

}