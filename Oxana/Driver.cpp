// The graphical driver library
#include "source/GraphicalDriver.h"

// The user's custom configuration for this driver
#include "user/User.h"

int main(int argc, char** argv)
{
  // Used to configure the GUI
  GraphicalDriver::GUI::Settings settings;

  // Construct the gui
  GraphicalDriver::GUI gui(settings);

  // Add custom stuff by user
  Customize(argc, argv, gui); 

  // Run it!
  gui.Run();

  return 0;
}