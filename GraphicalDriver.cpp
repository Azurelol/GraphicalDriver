#include "GraphicalDriver.h"

namespace Azure
{
  GraphicalDriver::GraphicalDriver(const Settings & settings) : 
    window(sf::VideoMode(settings.width, settings.height), settings.title),
    settings(settings)
  {
  }

  GraphicalDriver::Settings::Settings() : 
    title("Graphical Driver"),
    framerate(60),
    width(640),
    height(480)
  {
  }


  void GraphicalDriver::Run()
  {
    //sf::RenderWindow window(sf::RenderWindow(sf::VideoMode(settings.width, settings.height), settings.title))
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;

    while (window.isOpen())
    {
      sf::Event event;
      while (window.pollEvent(event))
      {
        ImGui::SFML::ProcessEvent(event);
        if (event.type == sf::Event::Closed)
          window.close();
      }

      ImGui::SFML::Update(window, deltaClock.restart());
      Draw();

      const sf::Color bgColor = sf::Color::White;
      window.clear(bgColor);
      ImGui::SFML::Render(window);
      window.display();
    }

    // Once the window has closed, shutdown
    ImGui::SFML::Shutdown();
  }

  void GraphicalDriver::Draw()
  {    
    ImGui::Button("Hi");
  }



}