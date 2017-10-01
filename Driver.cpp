#include "GraphicalDriver.h"

using namespace Azure;

int main ()
{
  GraphicalDriver::Settings settings;
  settings.title = "Graphical Driver Test, Go!";
  GraphicalDriver gui(settings);
  gui.Run();
}