#pragma once

#include "GraphicalDriverInclude.h"

// Include
#include "Window.h"
#include "WatchVariable.h"

namespace GraphicalDriver
{
  class GUI
  {
    public:
    // ------------------ Definitions ----------------/
    using StepFunction = std::function<bool()>;
    
    // Default settings for the window rendering
    struct Settings
    {
      std::string title;
      unsigned framerate;
      int width;
      int height;
      StepFunction stepFunction;
      bool showSTDOUT;
      std::string stdoutCaptureFile;
      // The maximum amount of steps to record
      unsigned stepBufferSize;
      bool showOverlay;
      int stepMultiplier;

      Settings();
    };

    Settings settings;

    // ------------------ Methods ----------------/
    // Constructor that takes a settings object
    GUI(const Settings& settings);
    // Starts running the driver
    void Run();
    // Notifies the user when a given condition is true, printing a message
    void AddNotification(const std::string& title, NotificationWindow::ConditionalFunction conditionFn, NotificationWindow::MessageFunction messageFn, bool persistent = false);
    // Adds a custom logging window
    void AddLog(std::string title, LogFunction printFn);
    // Adds a custom map window
    void AddMap(std::string title, ColorMap  map);
    // Tracks a variable (by having the user provide a reference to it)
    template <typename ReferenceType>
    void Watch(std::string name, ReferenceType* variable)
    { 
      WatchVariable<ReferenceType> watchedVariable(name, variable);
      watchVariables.push_back(std::make_shared<WatchVariable<ReferenceType>>(watchedVariable));
    }
    // Tracks a value (by having the user provide a custom print function of it)
    void Watch(std::string name, LogFunction printValueFn);
    // Tracks and plots a float-castable variable over stepss
    template <typename NumericType>
    void PlotLine(std::string name, NumericType* variable)
    {
      PlotVariable<NumericType> plot(name, variable);
      plotVariables.push_back(std::make_shared<PlotVariable<NumericType>>(plot));
    }

    private:    
    // ------------------ Members ----------------/
    sf::RenderWindow window;
    FILE* outputFile;
    std::stringstream outputStream;
    ImGuiTextBuffer log;

    // @TODO: Use inheritance instead with virtual draw method?
    std::vector<LogWindow> loggingWindows;
    std::vector<NotificationWindow> notifications;
    std::vector<ColorMapWindow> colorMapWindows;
    std::vector<std::shared_ptr<WatchVariableBase>> watchVariables;
    std::vector<std::shared_ptr<PlotVariableBase>> plotVariables;;

    unsigned steps;
    int currentStep;
    bool isDone;
    bool isAutomatic;

    // ------------------ Methods ----------------/
    void Update();
    void OnStep();
    void Draw();

    void DrawControls();
    void DrawGrid();
    void DrawWatcher();    
    void DrawSTDOUT();
    void DrawOverlay();

    void Initialize();
    void Shutdown();
    
    void SynchronizeBuffers();
    void RecordVariables();


  };

}