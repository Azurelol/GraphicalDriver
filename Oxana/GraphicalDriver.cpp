#include "GraphicalDriver.h"
#include <stdio.h>
#include <cstdio>


static void ShowExampleAppFixedOverlay(bool* p_open)
{
  const float DISTANCE = 10.0f;
  static int corner = 0;
  ImVec2 window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
  ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
  ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.3f));
  if (ImGui::Begin("Example: Fixed Overlay", p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
  {
    ImGui::Text("Simple overlay\nin the corner of the screen.\n(right-click to change position)");
    ImGui::Separator();
    ImGui::Text("Mouse Position: (%.1f,%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
    if (ImGui::BeginPopupContextWindow())
    {
      if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
      if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
      if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
      if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
      ImGui::EndPopup();
    }
    ImGui::End();
  }
  ImGui::PopStyleColor();
}


#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

namespace GraphicalDriver
{
  GUI::GUI(const Settings & settings) :
    window(sf::VideoMode(settings.width, settings.height), settings.title),
    settings(settings), steps(0), currentStep(steps), isAutomatic(false)
  {
  }

  GUI::Settings::Settings() :
    title("Graphical Driver"),
    framerate(60),
    width(1024),
    height(768),
    showSTDOUT(false),
    stdoutCaptureFile("GDOutput.txt"),
    stepBufferSize(100),
    showOverlay(true),
    stepMultiplier(5)
  {
  }


  void GUI::Initialize()
  {
    ImGui::SFML::Init(window);
    outputFile = std::freopen(settings.stdoutCaptureFile.c_str(), "w+", stdout);
    RecordVariables();
    SynchronizeBuffers();
  }

  void GUI::Shutdown()
  {
    // Once the window has closed, shutdown
    std::fclose(stdout);
    ImGui::SFML::Shutdown();
  }

  void GUI::SynchronizeBuffers()
  {
    for (auto& plot : plotVariables)
    {
      plot->bufferSize = settings.stepBufferSize;
      plot->history.reserve(plot->bufferSize);
    }

    for (auto& log : loggingWindows)
    {
      log.bufferSize = settings.stepBufferSize;
    }
  }

  void GUI::RecordVariables()
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


  void GUI::Run()
  {
    Initialize();

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
      Update();

      const sf::Color bgColor = sf::Color::White;
      window.clear(bgColor);
      ImGui::SFML::Render(window);
      window.display();
    }

    Shutdown();
  }


  void GUI::Update()
  {
    if (isAutomatic)
      OnStep();

    Draw();
  }

  void GUI::OnStep()
  {
    if (isDone)
      return;

    isDone = !settings.stepFunction();

    steps++;
    currentStep = steps;

    // Update all windows
    for (auto& log : loggingWindows)    
      log.Update();    

    for (auto& colorMap : colorMapWindows)
      colorMap.Update();

    for (auto& notification : notifications)
    {
      notification.Update();  
      if (notification.enabled)
        this->isAutomatic = false;
    }

    RecordVariables();
  }

  void GUI::Draw()
  {
    DrawControls();
    DrawWatcher();
    //DrawGrid();

    if (steps < 1)
      return;

    int bufferIndex = currentStep - 1;
    const int offset = steps - settings.stepBufferSize;
    if (offset > 0)
      bufferIndex = std::max(bufferIndex - offset, 0);

    // Draw all windows

    for (auto& window : loggingWindows)
      window.Draw((unsigned) bufferIndex);

    for (auto& window : colorMapWindows)
      window.Draw((unsigned) bufferIndex);

    for (auto& notification : notifications)
      notification.Draw((unsigned) bufferIndex);


  }

  void GUI::DrawControls()
  {
    // Controls the 'CurrentStep' slider
    int min = (steps > settings.stepBufferSize) ? steps - settings.stepBufferSize + 1 : 1;
    const int max = steps;

    ImGui::SetNextWindowSize(ImVec2(500, 200), ImGuiCond_FirstUseEver);
    ImGui::Begin("Controls");

    ImGui::Text("Steps");
    ImGui::Text(std::string("- Total: " + std::to_string(steps)).c_str());
    ImGui::Text(std::string("- Buffer size: " + std::to_string(settings.stepBufferSize)).c_str());
    ImGui::Text(std::string("- Step Multiplier: " + std::to_string(settings.stepMultiplier)).c_str());

    ImGui::Separator();

    ImGui::BeginGroup();

    if (ImGui::Button("Auto"))
    {
      isAutomatic = !isAutomatic;
    }
    ImGui::SameLine();

    // Take steps
    {
      if (ImGui::Button("Step Once"))
        OnStep();

      ImGui::SameLine();
      if (ImGui::Button("Step Multiple"))
      {
        for (int i = 0; i < settings.stepMultiplier; ++i)
          OnStep();
      }
    }
    ImGui::EndGroup();

    // Timeline
    if (steps > 0)
    {
      if (ImGui::Button("Backward"))
      {
        if (currentStep > min)
          currentStep--;
      }
      ImGui::SameLine();
      ImGui::SliderInt("", &currentStep, min, max);
      ImGui::SameLine();
      if (ImGui::Button("Forward"))
      {
        if (currentStep < max)
          currentStep++;
      }
    }

    ImGui::End();

  }

  void GUI::DrawGrid()
  {
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Example Grid");
    {
      ImDrawList* draw_list = ImGui::GetWindowDrawList();
      const int rows = 4;
      const int cols = 4;
      const ImVec2 screenSize = ImGui::GetWindowSize();
      const ImVec2 cellSize(screenSize.x / cols, screenSize.y / rows);
      static ImVec4 col(1.0f, 1.0f, 0.4f, 1.0f);
      const ImU32 col32 = ImColor(col);
      const float spacing = 2.0f;
      float sz = 64.0f;
      const ImVec2 p = ImGui::GetCursorScreenPos();

      float y = p.y + 4.0f;
      for (int i = 0; i < rows; ++i)
      {
        float x = p.x + 4.0f;
        for (int j = 0; j < cols; ++j)
        {
          draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col32); 
          if (ImGui::IsItemHovered());
          {
            ImGui::BeginTooltip();
            ImGui::TextUnformatted("Meow");
            ImGui::EndTooltip();
          }
          x += sz + spacing;            
          //draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + cellSize.x, y + cellSize.y), col32);
          //x += cellSize.x + spacing;
        }        
        y += sz + spacing;
      }

      // Now use a dummy to separate
      ImGui::Dummy(ImVec2((sz + spacing) * cols, (sz + spacing * 2) * rows));
      ImGui::Separator();
      ImGui::Text("data here...");
    }
    ImGui::End();
  }

  void GUI::DrawSTDOUT()
  {
    if (!settings.showSTDOUT)
      return;
  }

  void GUI::DrawWatcher()
  {
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Watcher");
    {

      // Watch      
      if (!watchVariables.empty())
      {
        ImGui::Columns(2);

        // Header
        ImGui::TextColored(sf::Color::Red, "Name");
        ImGui::NextColumn();
        ImGui::TextColored(sf::Color::Yellow, "Value");
        ImGui::NextColumn();
        ImGui::Separator();

        for (auto& var : watchVariables)
        {
          ImGui::Text(var->name.c_str());
          ImGui::NextColumn();
          ImGui::Text(var->lastValueToString.c_str());
          ImGui::NextColumn();
        }


        ImGui::Columns(1);
        ImGui::NewLine();
      }

      // Plots
      if (!plotVariables.empty())
      {
        ImGui::Columns(3);
        // Header
        {
          ImGui::TextColored(sf::Color::Red, "Name");
          ImGui::NextColumn();
          ImGui::TextColored(sf::Color::Yellow, "Value");
          ImGui::NextColumn();
          ImGui::TextColored(sf::Color::Yellow, "Plot");
          ImGui::NextColumn();
          ImGui::Separator();
        }

        for (auto& var : plotVariables)
        {
          // Name
          ImGui::Text(var->name.c_str());
          ImGui::NextColumn();
          // Value
          ImGui::Text(var->lastValueToString.c_str());
          ImGui::NextColumn();
          // Plot
          if (!var->history.empty())
            ImGui::PlotLines("", &var->history[0], var->history.size());
          ImGui::NextColumn();

        }
      }
    }

    ImGui::Columns(1);
    ImGui::End();
  }

  void GUI::AddLog(std::string title, LogFunction printFn)
  {
    LogWindow window;
    window.title = title;
    window.function = printFn;
    loggingWindows.push_back(window);
  }

  void GUI::AddMap(std::string title, ColorMap map)
  {
    ColorMapWindow window;
    window.title = title;
    window.map = map;
    colorMapWindows.push_back(window);
  }

  void GUI::Watch(std::string name, LogFunction printValueFn)
  {
    PrintVariableValue printVariable(name, printValueFn);
    watchVariables.push_back(std::make_shared<PrintVariableValue>(printVariable));
  }

  void GUI::AddNotification(const std::string& title, NotificationWindow::ConditionalFunction onCheckCondition, NotificationWindow::MessageFunction onNotify, bool persistent)
  {
    NotificationWindow window;
    window.enabled = false;
    window.title = title;
    window.onCheckCondition = onCheckCondition;
    window.onNotify = onNotify;   
    window.persistent = persistent;
    notifications.push_back(window);
  }


}