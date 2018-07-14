#pragma once

// Include
#include "GraphicalDriverInclude.h"
#include "Window.h"
#include "WatchVariable.h"
#include "Utilities.h"

namespace Oxana
{
  class GUI
  {
    public:
    // ------------------ Definitions ----------------/
    using StepFunction = std::function<bool()>;
    using DriverFunction = std::function<int(char, char*[])>;
    
    // Default settings for the window rendering
    struct Settings
    {
      // Title of the window
      std::string title;
      // At what framerate to run this window
      unsigned framerate;
      // Width of the window
      int width;
      // Height of the window
      int height;
      // The background color
      sf::Color background;
      // Invoked for every step of the simulation
      StepFunction onStep;
      // Validates the result of the simulation
      LogFunction onValidate;
      bool showSTDOUT;
      std::string stdoutCaptureFile;
      unsigned stepBufferSize;
      bool showOverlay;
      int stepMultiplier;
      DriverFunction originalDriver;
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
    void AddNotification(const std::string& title, bool& condition, NotificationWindow::MessageFunction messageFn, bool persistent = false);
    // Adds a custom logging window
    void AddLog(std::string title, LogFunction printFn);
    // Adds a custom map window
    void AddMap(std::string title, ColorMap map, ColorMap::Function gridFn);
    // Sets the primary set function
    void SetStep(const StepFunction& stepFn) { this->settings.onStep = stepFn; }
    // Adds a subset function
    void AddSubstep(const StepFunction& stepFn) {this-> substeps.push(stepFn); }
    // Observes a node, given a pointer to the node and a function to describe it
    template <typename NodeType>
    void ObserveNode(std::string title, GenericNodeDisplay<NodeType> nodeDisplay)
    {
      auto window = std::make_shared<NodeDisplayWindow>();
      window->title = title;      
      window->display.reset(new GenericNodeDisplay<NodeType>(nodeDisplay));           
      nodeDisplayWindows.push_back(window);
      windows.push_back(window.get());
    }

    // Tracks a variable (by having the user provide a reference to it)
    template <typename ValueType>
    void Watch(std::string name, ValueType* variable)
    { 
      WatchVariable<ValueType> watchedVariable(name, variable);
      watchVariables.push_back(std::make_shared<WatchVariable<ValueType>>(watchedVariable));
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
    enum class StepType
    {
      Once,
      Multiple,
      Forward,
      Backward,      
    };

    // ------------------ Members ----------------/
    std::unique_ptr<sf::RenderWindow> window;
    FILE* outputFile;
    std::stringstream outputStream;
    ImGuiTextBuffer log;
    std::stack<StepFunction> substeps;

    // Notifications
    std::vector<NotificationWindow> persistentNotifications;
    
    // @TODO: Use inheritance instead with virtual draw method?
    std::vector<Window*> windows;
    std::vector<LogWindow> loggingWindows;
    std::vector<ColorMapWindow> colorMapWindows;
    std::vector<std::shared_ptr<NodeDisplayWindow>> nodeDisplayWindows;
    std::vector<std::shared_ptr<WatchVariableBase>> watchVariables;
    std::vector<std::shared_ptr<PlotVariableBase>> plotVariables;

    unsigned steps;
    int currentStep;
    bool isDone;
    bool isAutomatic;

    // ------------------ Methods ----------------/
    void Update();
    void OnStep();
    void Draw();
    void Step(const StepType& type);
    void OnInput(sf::Keyboard::Key key);
    void OnFinished();

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