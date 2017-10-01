#pragma once

// STL
#include <functional>
#include <sstream>
#include <string>

// SFML
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

// ImGUI
#include <ImGui\imgui.h>
#include <ImGui\imgui-SFML.h>

namespace Azure
{
  class GraphicalDriver
  {
    public:
    // ------------------ Definitions ----------------/
    using InputStream = std::stringstream;
    // Default settings for the window rendering
    struct Settings
    {
      std::string title;
      unsigned framerate;
      int width;
      int height;
      Settings();
    };

    // ------------------ Methods ----------------/
    GraphicalDriver(const Settings& settings);
    void Run();

    private:    
    // ------------------ Members ----------------/
    sf::RenderWindow window;
    Settings settings;

    // ------------------ Methods ----------------/
    void Draw();


  };

}